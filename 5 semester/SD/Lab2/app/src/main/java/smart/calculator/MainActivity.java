package smart.calculator;

import android.content.pm.ActivityInfo;
import android.graphics.Color;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import java.util.List;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    final int MENU_CLEAR_ID = 1;
    final int MENU_QUIT_ID = 2;

    LinearLayout llMain;

    TextView tvDisplay;

    Button btn0, btn1, btn2, btn3, btn4, btn5, btn6, btn7, btn8, btn9;
    Button btnAdd, btnSub, btnMul, btnDiv, btnDot, btnPM, btnEq, btnSav, btnDel;

    String oper = "";
    String resultExpr = "";
    String screen = "";

    float num1 = 0;
    float num2 = 0;
    float result = 0;

    boolean afterCalc = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setRequestedOrientation (ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        llMain = (LinearLayout) findViewById(R.id.llMain);

        tvDisplay = (TextView) findViewById(R.id.tvDisplay);

        btn0 = (Button) findViewById(R.id.btn0);
        btn1 = (Button) findViewById(R.id.btn1);
        btn2 = (Button) findViewById(R.id.btn2);
        btn3 = (Button) findViewById(R.id.btn3);
        btn4 = (Button) findViewById(R.id.btn4);
        btn5 = (Button) findViewById(R.id.btn5);
        btn6 = (Button) findViewById(R.id.btn6);
        btn7 = (Button) findViewById(R.id.btn7);
        btn8 = (Button) findViewById(R.id.btn8);
        btn9 = (Button) findViewById(R.id.btn9);
        btnAdd = (Button) findViewById(R.id.btnAdd);
        btnSub = (Button) findViewById(R.id.btnSub);
        btnMul = (Button) findViewById(R.id.btnMul);
        btnDiv = (Button) findViewById(R.id.btnDiv);
        btnDot = (Button) findViewById(R.id.btnDot);
        btnPM = (Button) findViewById(R.id.btnPM);
        btnEq = (Button) findViewById(R.id.btnEq);
        btnSav = (Button) findViewById(R.id.btnSav);
        btnDel = (Button) findViewById(R.id.btnDel);

        btn0.setOnClickListener(this);
        btn1.setOnClickListener(this);
        btn2.setOnClickListener(this);
        btn3.setOnClickListener(this);
        btn4.setOnClickListener(this);
        btn5.setOnClickListener(this);
        btn6.setOnClickListener(this);
        btn7.setOnClickListener(this);
        btn8.setOnClickListener(this);
        btn9.setOnClickListener(this);
        btnAdd.setOnClickListener(this);
        btnSub.setOnClickListener(this);
        btnMul.setOnClickListener(this);
        btnDiv.setOnClickListener(this);
        btnDot.setOnClickListener(this);
        btnPM.setOnClickListener(this);
        btnEq.setOnClickListener(this);
        btnSav.setOnClickListener(this);
        btnDel.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        float lastNum = 0;

        if (afterCalc)
        {
            if (!screen.isEmpty())
                lastNum = Float.parseFloat(screen);
            screen = "";
        }

        switch (v.getId()) {
            case R.id.btn0:
                if (!screen.equals("0"))
                    screen += "0";
                afterCalc = false;
                break;
            case R.id.btn1:
                if (screen.equals("0"))
                    screen = "1";
                else screen += "1";
                afterCalc = false;
                break;
            case R.id.btn2:
                if (screen.equals("0"))
                    screen = "2";
                else screen += "2";
                afterCalc = false;
                break;
            case R.id.btn3:
                if (screen.equals("0"))
                    screen = "3";
                else screen += "3";
                afterCalc = false;
                break;
            case R.id.btn4:
                if (screen.equals("0"))
                    screen = "4";
                else screen += "4";
                afterCalc = false;
                break;
            case R.id.btn5:
                if (screen.equals("0"))
                    screen = "5";
                else screen += "5";
                afterCalc = false;
                break;
            case R.id.btn6:
                if (screen.equals("0"))
                    screen = "6";
                else screen += "6";
                break;
            case R.id.btn7:
                if (screen.equals("0"))
                    screen = "7";
                else screen += "7";
                afterCalc = false;
                break;
            case R.id.btn8:
                if (screen.equals("0"))
                    screen = "8";
                else screen += "8";
                afterCalc = false;
                break;
            case R.id.btn9:
                if (screen.equals("0"))
                    screen = "9";
                else screen += "9";
                afterCalc = false;
                break;
            case R.id.btnPM:
                if (!screen.equals("0")
                        && !screen.isEmpty()) {
                    if (screen.charAt(0) == '-')
                        screen = screen.substring(1);
                    else screen = "-" + screen;
                    afterCalc = false;
                }
                break;
            case R.id.btnDot:
                if (!TextUtils.isEmpty(tvDisplay.getText().toString())) {
                    if (!screen.contains("."))
                        screen += ".";
                    afterCalc = false;
                }
                break;
            case R.id.btnDel:
                if (!screen.isEmpty()) {
                    screen = screen.substring(0, screen.length()-1);
                }
                break;
            case R.id.btnAdd:
                if (!TextUtils.isEmpty(tvDisplay.getText().toString())) {
                    if (afterCalc)
                        num1 = lastNum;
                    else num1 = Float.parseFloat(screen);
                    afterCalc = false;
                    screen = "";
                    oper = "+";
                }
                break;
            case R.id.btnSub:
                if (!TextUtils.isEmpty(tvDisplay.getText().toString())) {
                    if (afterCalc)
                        num1 = lastNum;
                    else num1 = Float.parseFloat(screen);
                    afterCalc = false;
                    screen = "";
                    oper = "-";
                }
                break;
            case R.id.btnMul:
                if (!TextUtils.isEmpty(tvDisplay.getText().toString())) {
                    if (afterCalc)
                        num1 = lastNum;
                    else num1 = Float.parseFloat(screen);
                    afterCalc = false;
                    screen = "";
                    oper = "*";
                }
                break;
            case R.id.btnDiv:
                if (!TextUtils.isEmpty(tvDisplay.getText().toString())) {
                    if (afterCalc)
                        num1 = lastNum;
                    else num1 = Float.parseFloat(screen);
                    afterCalc = false;
                    screen = "";
                    oper = "/";
                }
                break;
            case R.id.btnEq:
                if (!TextUtils.isEmpty(tvDisplay.getText().toString())
                        && !oper.isEmpty()) {
                    num2 = Float.parseFloat(screen);
                    switch (oper) {
                        case "+":
                            result = num1 + num2;
                            resultExpr = num1 + " + " + num2 + " = " + result;
                            screen = Float.toString(result);
                            break;
                        case "-":
                            result = num1 - num2;
                            resultExpr = num1 + " - " + num2 + " = " + result;
                            screen = Float.toString(result);
                            break;
                        case "*":
                            result = num1 * num2;
                            resultExpr = num1 + " * " + num2 + " = " + result;
                            screen = Float.toString(result);
                            break;
                        case "/":
                            if (num2 != 0) {
                                result = num1 / num2;
                                resultExpr = num1 + " / " + num2 + " = " + result;
                                screen = Float.toString(result);
                            }
                            else {
                                num1 = num2 = result = 0;
                                screen = "";
                                Toast.makeText(
                                        getApplicationContext(),
                                        "Can't divide by zero!",
                                        Toast.LENGTH_SHORT).show();
                            }
                            break;
                    }

                    afterCalc = true;
                }
                break;
            case R.id.btnSav:
                if (!resultExpr.isEmpty()) {
                    LinearLayout.LayoutParams lParams = new LinearLayout.LayoutParams(
                            LinearLayout.LayoutParams.MATCH_PARENT,
                            LinearLayout.LayoutParams.WRAP_CONTENT);
                    lParams.bottomMargin = 5;

                    TextView tvSav = new TextView(this);

                    tvSav.setBackgroundColor(Color.parseColor("#cecece"));
                    tvSav.setText(resultExpr);
                    tvSav.setTextColor(Color.BLACK);
                    tvSav.setTextSize(25);
                    tvSav.setGravity(Gravity.CENTER_HORIZONTAL);

                    llMain.addView(tvSav, lParams);
                    resultExpr = "";
                }
                break;
        }

        tvDisplay.setText(screen);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        menu.add(0, MENU_CLEAR_ID, 0, "Clear saved results");
        menu.add(0, MENU_QUIT_ID, 0, "Quit");
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case MENU_CLEAR_ID:
                llMain.removeAllViews();
                Toast.makeText(getApplicationContext(),
                        "Cleared!",
                        Toast.LENGTH_SHORT).show();
                break;
            case MENU_QUIT_ID:
                finish();
                break;
        }
        return super.onOptionsItemSelected(item);
    }
}
