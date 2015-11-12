using System;

class Vehicle
{
    static string vehicle_type = "Cars";
    string car_brand;
    string color;
    int speed;
    string country;
    bool fuel;

    public Vehicle(string car_brand, string color, int speed, string country, bool fuel)
    {
        this.car_brand = car_brand;
        this.color = color;
        this.speed = speed;
        this.country = country;
        this.fuel = fuel;
    }
    public static string VehicleType
    {
        get { return vehicle_type; }
    }
    public string CarBrand
    {
        get { return car_brand; }
        set { car_brand = value; }
    }
    public string Color
    {
        get { return color; }
        set { color = value; }
    }
    public int Speed
    {
        get { return speed; }
        set { speed = value; }
    }
    public string Country
    {
        get { return country; }
        set { country = value; }
    }
    public bool Fuel
    {
        get { return fuel; }
        set { fuel = value; }
    }
    public void StartTheCar()
    {
        Console.WriteLine("You should choose the car you want to start!");
    }
    public void StartTheCar(Vehicle car)
    {
        if (car.fuel == true)
            Console.WriteLine("Your car is started!");
        else
            Console.WriteLine("Your car is not fueled!");
    }
    public void FuelTheCar(Vehicle car)
    {
        car.fuel = true;
    }
    public void Information()
    {
        Console.WriteLine("Car brand: " + car_brand);
        Console.WriteLine("Country: " + country);
        Console.WriteLine("Color: " + color);
        Console.WriteLine("Speed: " + speed + " kmph");
        if (fuel == true)
            Console.WriteLine("FUELED");
        if (fuel == false)
            Console.WriteLine("NOT FUELED");
        Console.WriteLine("");
    }
}

class Program
{
    static void Main()
    {
        ConsoleKeyInfo ch;
        Vehicle choosen_car = new Vehicle("", "", 0, "", false);
        Vehicle car1 = new Vehicle("Peugeot", "Green", 120, "France", false);
        Vehicle car2 = new Vehicle("Opel", "Black", 150, "Germany", false);
        
        while (true)
        {
            Console.Clear();
            Console.WriteLine("Vehicle type:");
            Console.WriteLine(Vehicle.VehicleType);
            Console.WriteLine("");
            car1.Information();
            car2.Information();
            Console.WriteLine("");
            Console.WriteLine("1 - Start the car");
            Console.WriteLine("2 - Choose the car");
            Console.WriteLine("3 - Fuel the car");
            Console.WriteLine("0 - Exit");
            ch = Console.ReadKey();
            if (ch.KeyChar == '1')
            {
                Console.Clear();
                if (choosen_car.CarBrand == "")
                    choosen_car.StartTheCar();
                else
                    choosen_car.StartTheCar(choosen_car);
            }
            if (ch.KeyChar == '2')
            {
                Console.Clear();
                Console.WriteLine("What car do you want to choose?");
                Console.WriteLine("1 - Peugeot");
                Console.WriteLine("2 - Opel");
                ch = Console.ReadKey();
                if (ch.KeyChar == '1')
                    choosen_car = car1;
                if (ch.KeyChar == '2')
                    choosen_car = car2;
            }
            if (ch.KeyChar == '3')
            {
                Console.Clear();
                Console.WriteLine("What car do you want to fill?");
                Console.WriteLine("1 - Peugeot");
                Console.WriteLine("2 - Opel");
                ch = Console.ReadKey();
                if (ch.KeyChar == '1')
                    car1.FuelTheCar(car1);
                if (ch.KeyChar == '2')
                    car2.FuelTheCar(car2);
            }
            if (ch.KeyChar == '0')
                break;
            Console.ReadKey();
        }
    }
}