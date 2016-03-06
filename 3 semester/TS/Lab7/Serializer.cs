using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;

namespace Lab7
{
    class Serializer
    {
        public FileStream stream = null;
        protected StreamWriter writer = null;
        protected StreamReader reader = null;
        protected int spaces = 0;

        public Serializer(string _filename, bool _storemode)
        {
            if (_storemode)
            {
                stream = new FileStream(_filename, FileMode.Create);
                writer = new StreamWriter(stream);
            }
            else
            {
                stream = new FileStream(_filename, FileMode.Open);
                reader = new StreamReader(stream);
            }
        }

        public void Close()
        {
            if (writer != null)
            {
                writer.Close();
                writer = null;
            }

            if (reader != null)
            {
                reader.Close();
                reader = null;
            }

            if (stream != null)
            {
                stream.Close();
                stream = null;
            }
        }

        protected void WriteLine(string _key, string _line, bool _useconvert)
        {
            if (spaces > 0)
                writer.Write(new string(' ', spaces << 1));

            if (_useconvert)
            {
                _line = _line.Replace("&", "&amp;");
                _line = _line.Replace("\n", "&ln;");
                _line = _line.Replace("\r", "&lf;");
                _line = _line.Replace("{", "&bl;");
                _line = _line.Replace("}", "&br;");
                _line = _line.Replace("[", "&al;");
                _line = _line.Replace("]", "&ar;");
                _line = _line.Replace("(", "&ul;");
                _line = _line.Replace(")", "&ur;");
                _line = _line.Replace("=", "&eq;");
            }

            if (_key != null)
                writer.Write(_key + "=");

            writer.WriteLine(_line);
        }

        protected string ReadLine(out string _key, bool _useconvert)
        {
            string line;

            while ((line = reader.ReadLine()) != null)
            {
                if (line.Trim().Length > 0)
                    break;
            }

            if (line != null)
            {
                int eqpos = (line = line.TrimStart(null)).IndexOf('=');

                if (eqpos > 0)
                {
                    _key = line.Substring(0, eqpos);
                    line = line.Substring(eqpos + 1);
                }
                else _key = null;

                if (_useconvert)
                {
                    line = line.Replace("&ln;", "\n");
                    line = line.Replace("&lf;", "\r");
                    line = line.Replace("&bl;", "{");
                    line = line.Replace("&br;", "}");
                    line = line.Replace("&al;", "[");
                    line = line.Replace("&ar;", "]");
                    line = line.Replace("&ul;", "(");
                    line = line.Replace("&ur;", ")");
                    line = line.Replace("&eq;", "=");
                    line = line.Replace("&amp;", "&");
                }
            }
            else _key = null;

            return line;
        }

        public void WriteObject(object _object)
        {
            List<object> slist = new List<object>();
            WriteObject(_object, slist);
        }

        protected void WriteObject(object _object, List<object> _slist)
        {
            int refindex;

            if (_object == null)
            {
                WriteLine(null, typeof(object).FullName, true);
                WriteLine(null, "{", false);
            }
            else if((refindex = _slist.IndexOf(_object)) >= 0)
            {
                Type objtype = _object.GetType();

                WriteLine(null, objtype.FullName, true);
                WriteLine(null, "{", false);
                spaces++;
                WriteLine("reference", refindex.ToString(), false);
                spaces--;
            }
            else
            {
                _slist.Add(_object);

                Type objtype = _object.GetType();

                WriteLine(null, objtype.FullName, true);
                WriteLine(null, "{", false);
                spaces++;

                if (objtype.IsPrimitive || objtype.Equals(typeof(string)))
                {
                    WriteLine("value", _object.ToString(), true);
                }
                else if (objtype.IsArray)
                {
                    WriteLine(null, "[", false);
                    spaces++;

                    WriteLine("count", ((System.Array) _object).Length.ToString(), false);
                    WriteLine(null, "", false);

                    foreach (object item in ((System.Array)_object))
                    {
                        WriteObject(item, _slist);
                    }

                    spaces--;
                    WriteLine(null, "]", false);
                }
                else
                {                    
                    WriteLine(null, "[", false);
                    spaces++;

                    foreach (FieldInfo finfo in objtype.GetFields(BindingFlags.GetField | BindingFlags.SetField | BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance))
                    {
                        if (!finfo.IsLiteral && !finfo.IsInitOnly && !finfo.IsNotSerialized && finfo.FieldType.BaseType != null && !finfo.FieldType.BaseType.Equals(typeof(System.MulticastDelegate)))
                        {
                            WriteLine(null, finfo.Name, true);
                            WriteLine(null, "(", false);
                            spaces++;

                            WriteObject(finfo.GetValue(_object), _slist);

                            spaces--;
                            WriteLine(null, ")", false);
                        }
                    }

                    spaces--;
                    WriteLine(null, "]", false);
                }

                spaces--;                
            }

            WriteLine(null, "}", false);
        }

        protected object LoadObject(string _classname, List<object> _slist)
        {
            object resobject = null;
            string key = null;
            string line;

            if (_classname == null)
                _classname = ReadLine(out key, true);

            if (_classname == null)
                return null;

            if (key != null || ReadLine(out key, false) != "{" || key != null)
                throw new Exception("Bad header object signature");

            if ((line = ReadLine(out key, true)) != "}")
            {
                Type objtype = Type.GetType(_classname);

                if (key != null && key == "reference")
                {
                    int refindex = Convert.ToInt32(line);

                    if (refindex < 0 || refindex >= _slist.Count)
                        throw new Exception("Bad reference on previous object");

                    resobject = _slist[refindex];
                }
                else if (objtype.IsPrimitive)
                {
                    if (key != "value")
                        throw new Exception("Bad value for primitive object");

                    resobject = (object)objtype.InvokeMember("Parse",
                      BindingFlags.InvokeMethod | BindingFlags.Static | BindingFlags.Public,
                      null, null, new object[] { line });

                    _slist.Add(resobject);
                }
                else if (objtype.Equals(typeof(string)))
                {
                    if (key != "value")
                        throw new Exception("Bad value for primitive object");

                    resobject = line;
                    _slist.Add(resobject);
                }
                else if (line == "[" && key == null)
                {
                    if (objtype.IsArray)
                    {
                       if((line = ReadLine(out key, false)) == null || key != "count")
                           throw new Exception("Bad count for array object");

                       int count = Convert.ToInt32(line), index = 0;

                       resobject = Activator.CreateInstance(objtype, count);
                       _slist.Add(resobject);

                        while ((line = ReadLine(out key, true)) != "]" || key != null)
                            ((System.Array)resobject).SetValue(LoadObject(line, _slist), index++);                        
                    }
                    else
                    {
                        resobject = Activator.CreateInstance(objtype);
                        _slist.Add(resobject);

                        while ((line = ReadLine(out key, true)) != "]" || key != null)
                        {
                            if (line == null || key != null)
                                throw new Exception("Bad header field signature");

                            string fieldname = line;

                            if ((line = ReadLine(out key, true)) != "(" || key != null)
                                throw new Exception("Bad header object signature");

                            object fieldobj = LoadObject(null, _slist);
                            FieldInfo fiendinf = objtype.GetField(fieldname, BindingFlags.GetField | BindingFlags.SetField | BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance);

                            if (fiendinf != null)
                                fiendinf.SetValue(resobject, fieldobj);

                            if ((line = ReadLine(out key, true)) != ")" || key != null)
                                throw new Exception("Bad tail object signature");
                        }                        
                    }
                }
                else throw new Exception("Bad header content signature");

                if ((line = ReadLine(out key, true)) != "}" || key != null)
                    throw new Exception("Bad tail object signature");
            }
            else if (key != null)
                throw new Exception("Bad tail object signature");

            return resobject;
        }

        public object ReadObject()
        {
            List<object> slist = new List<object>();
            return LoadObject(null, slist);
        }
    }
}