# HOTEL-MANAGEMENT-SYSTEM-CPP
A hotel management system built in C++ using OOP.

**1. Introduction:**
This project is a simple Hotel Management System coded in C++. It is designed to help manage hotel rooms, bookings, and user accounts. The system consists of two types of users: Admin and Customer. Admins can manage rooms (add, update, delete), while Customers can sign up, log in, view rooms, book rooms, and cancel bookings.

**2. Objectives:**
The objective of this project is to design and implement a real-world Hotel     Management System using Object-Oriented Programming in C++.
- Support admin and customer functionalities.
- Allows data to be stored by the software in between iterations.

**3. Main Features:**
Admin login with pre-defined credentials.
- Customer signup and login system.
- Room management (add, update, delete, view, search).
- Booking and cancellation by customers.
- File handling for saving and loading data.
- Input validation and error handling.

**4. System Design:**
The system is built using OOP concepts like inheritance, polymorphism, encapsulation, and abstraction. The main classes are:

- User (Base class): Contains username and password.
- Admin & Customer (Derived from User): Represent different users.
- Room Class: Represents a hotel room with details like number, type, price, availability.
- Booking Class: Stores info of customer and booked room.
- HotelManagement Class: Main class that controls the system's logic and menus.

**5. How the System Works:**
Following are the steps how system works:
1. Admin logs in and manages rooms.
2. Customers sign up or log in.
3. Customers can view/search rooms.
4. Customers can book or cancel rooms.
5. Data is saved in text files (rooms.txt, customers.txt).

