# Filee
Filee is a high-performance, cross-platform, and open-source Local Area Network (LAN) file sharing tool licensed under the GPLv3 license.

## About End-to-End Encryption
Please be noted that by default, Qt does not include OpenSSL due to legal restrictions. Filee, as an open-source project licensed under the GPLv3 license (which is required by Qt), are restricted from including OpenSSL in the project for the same reason as Qt. If you want your build to support end-to-end encryption, you must build OpenSSL for the platform you desire.

### Building for Windows
Please follow Qt's official guide on linking to OpenSSL. Link to the official documentation: https://doc.qt.io/qt-5/windows-requirements.html.

### Building for Android
Please follow Qt's official guide on linking to OpenSSL. Link to the official documentation: https://doc.qt.io/qt-5/android-openssl-support.html.

### Creating Self-Signed Certificates
Please place the private key and the certificate in the application's root folder. 

```bash
$ openssl req -x509 -nodes -days 30 -newkey rsa:1024 -keyout private.pem -out cert.pem
```

Additionally, due to all the certificates are self-signed, please also place the receiver's certificate file in the sender's root folder as well and name it `servercert.pem`. If you wish to ignore the verification, you may change the sender's verification mode to `QSslSocket::VerifyNone` where `QSslSocket` will not request a certificate from the peer. You can set this mode if you are not interested in the identity of the other side of the connection. The connection will still be encrypted, and your socket will still send its local certificate to the peer if it's requested.

#### Certificate Directory for Android
On Android, the certificate and private key should be placed under (by default), `/data/user/0/<package_name>/files`, in other words, `Android/data/<package_name>/files`.