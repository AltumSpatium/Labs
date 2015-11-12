using System;

struct VehicleInfo
{
    public string brand;
    public string color;
    public int speed;
    public bool fuel;
}

enum Vehicles{
    car = 0,
    motorcycle = 1,
    powerboat = 2 } 

abstract class Vehicle
{
    protected VehicleInfo info = new VehicleInfo();

    public Vehicle(string brand, string color, int speed, bool fuel)
    {
        info.brand = brand;
        info.color = color;
        info.speed = speed;
        info.fuel = fuel;
    }

    public string VehicleBrand
    {
        get { return info.brand; }
        set { info.brand = value; }
    }
    public string Color
    {
        get { return info.color; }
        set { info.color = value; }
    }
    public int Speed
    {
        get { return info.speed; }
        set { info.speed = value; }
    }
    public bool Fuel
    {
        get { return info.fuel; }
        set { info.fuel = value; }
    }
    public virtual void StartTheVehicle()
    {
        Console.WriteLine("You should choose the vehicle you want to start!");
    }
    public virtual void StartTheVehicle(Vehicle _vehicle)
    {
        if (_vehicle.Fuel == true)
            Console.WriteLine("Your vehicle is started!");
        else
            Console.WriteLine("Your vehicle is not fueled!");
    }
    public virtual void FuelTheVehicle(Vehicle _vehicle)
    {
        _vehicle.Fuel = true;
    }
    abstract public void Information();
}

class Car : Vehicle
{
    public Car(string brand, string color, int speed, bool fuel):
        base(brand, color, speed, fuel)
    {
    }
    public override void StartTheVehicle()
    {
        Console.WriteLine("You should choose the car you want to start!");
    }
    public override void StartTheVehicle(Vehicle car)
    {
        if (car.Fuel == true)
            Console.WriteLine("Your car is started!");
        else
            Console.WriteLine("Your car is not fueled!");
    }

    public override void FuelTheVehicle(Vehicle car)
    {
        car.Fuel = true;
    }
    public override void Information()
    {
        Console.WriteLine("Car brand: " + VehicleBrand);
        Console.WriteLine("Color: " + Color);
        Console.WriteLine("Speed: " + Speed + " kmph");
        if (Fuel == true)
            Console.WriteLine("FUELED");
        if (Fuel == false)
            Console.WriteLine("NOT FUELED");
        Console.WriteLine("");
    }
}

class Motorcycle : Vehicle
{
    public Motorcycle(string brand, string color, int speed, bool fuel) :
        base(brand, color, speed, fuel)
    {
    }
    public override void StartTheVehicle()
    {
        Console.WriteLine("You should choose the motorcycle you want to start!");
    }
    public override void StartTheVehicle(Vehicle motorcycle)
    {
        if (motorcycle.Fuel == true)
            Console.WriteLine("Your motorcycle is started!");
        else
            Console.WriteLine("Your motorcycle is not fueled!");
    }
    public override void FuelTheVehicle(Vehicle motorcycle)
    {
        motorcycle.Fuel = true;
    }
    public override void Information()
    {
        Console.WriteLine("Motorcycle brand: " + VehicleBrand);
        Console.WriteLine("Color: " + Color);
        Console.WriteLine("Speed: " + Speed + " kmph");
        if (Fuel == true)
            Console.WriteLine("FUELED");
        if (Fuel == false)
            Console.WriteLine("NOT FUELED");
        Console.WriteLine("");
    }
}

class Powerboat : Vehicle
{
    public Powerboat(string brand, string color, int speed, bool fuel) :
        base(brand, color, speed, fuel)
    {
    }
    public override void StartTheVehicle()
    {
        Console.WriteLine("You should choose the powerboat you want to start!");
    }
    public override void StartTheVehicle(Vehicle powerboat)
    {
        if (powerboat.Fuel == true)
            Console.WriteLine("Your powerboat is started!");
        else
            Console.WriteLine("Your powerboat is not fueled!");
    }
    public override void FuelTheVehicle(Vehicle powerboat)
    {
        powerboat.Fuel = true;
    }
    public override void Information()
    {
        Console.WriteLine("Powerboat brand: " + VehicleBrand);
        Console.WriteLine("Color: " + Color);
        Console.WriteLine("Speed: " + Speed + " kmph");
        if (Fuel == true)
            Console.WriteLine("FUELED");
        if (Fuel == false)
            Console.WriteLine("NOT FUELED");
        Console.WriteLine("");
    }
}

class Program
{
    static void Main()
    {
        ConsoleKeyInfo ch;
        ConsoleKeyInfo ch1;
        Vehicle choosen_car = new Car("", "", 0, false);
        Vehicle car1 = new Car("Peugeot", "Blue", 120, false);
        Vehicle car2 = new Car("Opel", "Black", 150, false);
        Vehicle choosen_motorcycle = new Motorcycle("", "", 0, false);
        Vehicle motorcycle1 = new Motorcycle("Yamaha", "Blue", 140, false);
        Vehicle motorcycle2 = new Motorcycle("Harley Davidson", "Red", 170, false);
        Vehicle choosen_powerboat = new Powerboat("", "", 0, false);
        Vehicle powerboat1 = new Powerboat("Windboat", "White", 50, false);
        Vehicle powerboat2 = new Powerboat("Shark boats", "Green", 60, false);

        while (true)
        {
            Console.Clear();
            Console.WriteLine("Choose the vehicle: ");
            Console.WriteLine("");
            Console.WriteLine("1 - Car");
            Console.WriteLine("2 - Motorcycle");
            Console.WriteLine("3 - Powerboat");
            Console.WriteLine("0 - Exit");
            ch = Console.ReadKey();
            if (ch.KeyChar == '1')
            {
                while (true)
                {
                    Console.Clear();
                    car1.Information();
                    car2.Information();
                    Console.WriteLine("");
                    Console.WriteLine("1 - Start the car");
                    Console.WriteLine("2 - Choose the car");
                    Console.WriteLine("3 - Fuel the car");
                    Console.WriteLine("0 - Back");
                    ch1 = Console.ReadKey();
                    if (ch1.KeyChar == '1')
                    {
                        Console.Clear();
                        if (choosen_car.VehicleBrand == "")
                            choosen_car.StartTheVehicle();
                        else
                            choosen_car.StartTheVehicle(choosen_car);
                    }
                    if (ch1.KeyChar == '2')
                    {
                        Console.Clear();
                        Console.WriteLine("What car do you want to choose?");
                        Console.WriteLine("1 - Peugeot");
                        Console.WriteLine("2 - Opel");
                        ch1 = Console.ReadKey();
                        if (ch1.KeyChar == '1')
                            choosen_car = car1;
                        if (ch1.KeyChar == '2')
                            choosen_car = car2;
                    }
                    if (ch1.KeyChar == '3')
                    {
                        Console.Clear();
                        Console.WriteLine("What car do you want to fill?");
                        Console.WriteLine("1 - Peugeot");
                        Console.WriteLine("2 - Opel");
                        ch1 = Console.ReadKey();
                        if (ch1.KeyChar == '1')
                            car1.FuelTheVehicle(car1);
                        if (ch1.KeyChar == '2')
                            car2.FuelTheVehicle(car2);
                    }
                    if (ch1.KeyChar == '0')
                        break;
                    Console.ReadKey();
                }         
            }
            if (ch.KeyChar == '2')
            {
                while (true)
                {
                    Console.Clear();
                    motorcycle1.Information();
                    motorcycle2.Information();
                    Console.WriteLine("");
                    Console.WriteLine("1 - Start the motorcycle");
                    Console.WriteLine("2 - Choose the motorcycle");
                    Console.WriteLine("3 - Fuel the motorcycle");
                    Console.WriteLine("0 - Back");
                    ch1 = Console.ReadKey();
                    if (ch1.KeyChar == '1')
                    {
                        Console.Clear();
                        if (choosen_motorcycle.VehicleBrand == "")
                            choosen_motorcycle.StartTheVehicle();
                        else
                            choosen_motorcycle.StartTheVehicle(choosen_motorcycle);
                    }
                    if (ch1.KeyChar == '2')
                    {
                        Console.Clear();
                        Console.WriteLine("What motorcycle do you want to choose?");
                        Console.WriteLine("1 - Yamaha");
                        Console.WriteLine("2 - Harley Davidson");
                        ch1 = Console.ReadKey();
                        if (ch1.KeyChar == '1')
                            choosen_motorcycle = motorcycle1;
                        if (ch1.KeyChar == '2')
                            choosen_motorcycle = motorcycle2;
                    }
                    if (ch1.KeyChar == '3')
                    {
                        Console.Clear();
                        Console.WriteLine("What motorcycle do you want to fill?");
                        Console.WriteLine("1 - Yamaha");
                        Console.WriteLine("2 - Harley Davidson");
                        ch1 = Console.ReadKey();
                        if (ch1.KeyChar == '1')
                            motorcycle1.FuelTheVehicle(motorcycle1);
                        if (ch1.KeyChar == '2')
                            motorcycle2.FuelTheVehicle(motorcycle2);
                    }
                    if (ch1.KeyChar == '0')
                        break;
                    Console.ReadKey();
                }
            }
            if (ch.KeyChar == '3')
            {
                while (true)
                {
                    Console.Clear();
                    powerboat1.Information();
                    powerboat2.Information();
                    Console.WriteLine("");
                    Console.WriteLine("1 - Start the powerboat");
                    Console.WriteLine("2 - Choose the powerboat");
                    Console.WriteLine("3 - Fuel the powerboat");
                    Console.WriteLine("0 - Back");
                    ch1 = Console.ReadKey();
                    if (ch1.KeyChar == '1')
                    {
                        Console.Clear();
                        if (choosen_powerboat.VehicleBrand == "")
                            choosen_powerboat.StartTheVehicle();
                        else
                            choosen_powerboat.StartTheVehicle(choosen_powerboat);
                    }
                    if (ch1.KeyChar == '2')
                    {
                        Console.Clear();
                        Console.WriteLine("What powerboat do you want to choose?");
                        Console.WriteLine("1 - Windboat");
                        Console.WriteLine("2 - Shark boats");
                        ch1 = Console.ReadKey();
                        if (ch1.KeyChar == '1')
                            choosen_powerboat = powerboat1;
                        if (ch1.KeyChar == '2')
                            choosen_powerboat = powerboat2;
                    }
                    if (ch1.KeyChar == '3')
                    {
                        Console.Clear();
                        Console.WriteLine("What powerboat do you want to fill?");
                        Console.WriteLine("1 - Windboat");
                        Console.WriteLine("2 - Shark boats");
                        ch1 = Console.ReadKey();
                        if (ch1.KeyChar == '1')
                            powerboat1.FuelTheVehicle(powerboat1);
                        if (ch1.KeyChar == '2')
                            powerboat2.FuelTheVehicle(powerboat2);
                    }
                    if (ch1.KeyChar == '0')
                        break;
                    Console.ReadKey();
                }
            }
            if (ch.KeyChar == '0')
                break;
            Console.ReadKey();
        }
    }
}
