Purposes:
0) Support AES Encrypt/Decrypt
1) Support SM2 Encrypt/Decrypt
   AES Key bits is 192 bits or 256bits
2) 设计模式: Adaptor, Decorator, Bridge, Singleton, Abstract Factory.

目标:
0) 支持AES 加密解密
  AES 密钥为 128bits 或 256bits
  AES Encryptor 为 CBC, ECB, CTR

  使用
  Key* key = new SymmetricKey(128);
  // Key* key = new SymmetricKey(256);
  // key->ToString();
  SymmetricEncryptor* encryptor = new ECBEncryptor(key);
  // SymmetricEncryptor* cbc_encryptor = new CBCEncryptor(key, "16 bytes iv");
  // SymmetricEncryptor* ctr_encryptor = new CTREncryptor(key);
  InputStream* input_stream = new StringInputStream("Hello, world");
  OuputStream* encrypted_out= new StringOuputStream;  

  Status s = SymmetricEncrypt(encryptor, input_stream, encrypted_out);
  if (!s.ok()) { return 1; }
