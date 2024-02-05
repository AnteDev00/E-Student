# E-Student - project

## Note:
The project was developed in RAD Studio - C++ Builder. Git and GitHub were not used. This is solely for code review purposes.

**All the Source files and Binary files are in separate branches!**

## Project Description

EStudents is an application developed in the C++ Builder environment for teachers and students. The client application allows user verification through secure communication with the server application. Professors can enter, delete, and review grades for their subjects, while students have access to their own grade overview. The system utilizes the AES algorithm for encryption and digital signatures to ensure secure data exchange between the client and server.

The server application, besides controlling and verifying data, dynamically generates secret keys for additional security. The project uses an MS Access database to store information about users, subjects, and grades. Notable features include filtering, sorting, and theme management. Security measures such as password hashing and the application of digital signatures to ensure data integrity have also been implemented. All these features make the EStudents project a robust system for monitoring and managing grades in a school environment.

## Project Structure

- **Client Application:** Provides a user interface for professors and students.
- **Server Application:** Manages user authentication, data verification, and secure communication.

## Technologies Used

- C++ Builder
- MS Access Database
- AES and RSA Encryption
- Digital Signatures
- Hashing algorithms (+ Salt and Pepper)
- TCP Protocol (Internet)

## Key Features

- Secure user authentication
- AES encryption for data security
- Digital signatures for data integrity
- Dynamic generation of secret keys
- Filtering and sorting functionalities
- Theme management

## Images

Client.exe (Login form) + Server.exe

![Image1](https://github.com/AnteDev00/EStudent-Projekt/assets/151842550/1c7345b3-a1fc-4fcb-8043-3d16a08f2046)

Client.exe (Professor form)

![Image2](https://github.com/AnteDev00/EStudent-Projekt/assets/151842550/dc4c0e8b-fe57-4b64-ad86-e295711785df)

Client.exe (Student form)

![Image3](https://github.com/AnteDev00/EStudent-Projekt/assets/151842550/4bbdfb79-c6d4-48f9-817d-6baeb1b0ce52)

Client.exe (User authentication)

![Image](https://github.com/AnteDev00/EStudent-Projekt/assets/151842550/bfd82a67-ab26-4a32-94ca-53d8aba58b08)

Client.exe (Theme change (light/dark))

![Image](https://github.com/AnteDev00/EStudent-Projekt/assets/151842550/ffaa48aa-ee31-4ddb-b758-27da8080e180)

Client.exe (Server.exe records *everyhing*)

![Image](https://github.com/AnteDev00/EStudent-Projekt/assets/151842550/a74dfc76-68ab-468a-9e0d-f753d8b3fc6f)

## And much more.. Files are in separate _branches_.
