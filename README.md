# Filee
Filee is a high-performance, cross-platform, and open-source Local Area Network (LAN) file sharing tool licensed under the GPLv3 license.

<img src="https://raw.githubusercontent.com/RapDoodle/Filee/main/screenshots/01.jpg" height="350"/> <img src="https://raw.githubusercontent.com/RapDoodle/Filee/main/screenshots/02.jpg" height="350"/> <img src="https://raw.githubusercontent.com/RapDoodle/Filee/main/screenshots/03.jpg" height="350"/>

## Demo
YouTube: https://youtu.be/iwnmr1nOero

## The Design and Implementation

### Broadcast Module
The `broadcast` module is served as notifying others of the devices’ presence on the network so that the users don’t have to enter the IP address.
- The `Broadcaster` sends UDP broadcast messages on port 6816.
- The `BroadcastReceiver` listens to messages on the UDP port 6816.
- The `Broadcaster` will notify the `OnlineDeviceModel` in the `models` module to update the list accordingly.

### Transfer Module
The transfer module is responsible for transferring files between devices.
- The `FileSender` sends files.
- `FileSender` is encapsulated by `FileSenderSession` to address issues like errors that would occur in a mobile environment.
- The `FileReceiver` receives files.
- The `FileReceiver` is created by `FileReceiveServer`’s master socket upon new connections.
- Both the `FileSender` and `FileReceiver` are inherited from `FileTransferPeer`.
- The `FileTransferPeer` contains the standards of the file transfer protocol
    - The definition of the header
    - The structure of TCP packet’s payload
    - Provides a unified interface for the sender and receiver to send and receive packets

### Model Nodule
The model module contains model codes used in Qt’s Model-View system.

### Utils Module
The utils module contains utility functions used throughout the application.
- The `AndroidUtils` class contains functions specific to Android.
    - Parse the correct file name
    - Reading the file extension from the file signature (binary)

### Others
- The `GuiController` handles the interaction between the frontend and the backend.

