/*
 * PEM-encoded client certificate.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----"
 * "...base64 data..."
 * "-----END CERTIFICATE-----";
 */
static const char clientcredentialCLIENT_CERTIFICATE_PEM[] = 
"-----BEGIN CERTIFICATE-----\n"
"MIIDWjCCAkKgAwIBAgIVALdooRi5yiTlupbOfKmHseGtZccQMA0GCSqGSIb3DQEB\n"
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n"
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xOTA1MDkxMzMx\n"
"MTVaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n"
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDUsT66FMuRpyCj4SA+\n"
"9XKPMSAH2YC9jSvd9Lr5Kb54ShIsL+5iigoufLSNvczWSzdHaoXkn0Axq7diFxhr\n"
"aPVM35XTwJXIKMwqnhJbchHG+SRf4kL2L9ZpuoXaSc1lamTWUca6xQg49eUk2g2E\n"
"EnxtBkZdk+FN9O37NgfA6o3VJBTW3zWbWiLPatqvRDZLDykymTUKEGKAT62po3yg\n"
"BEjgFmoolSow886yreYjj954uBVuW6fzpc4lQ5UenYEpDQfC/pnKWyNEvWI0pSOE\n"
"u5dp/7B3Do4ktjpeSnm9vtA1704Q2vQsyECAauwmOBvPfy1KsgFEft/kAw95IKzh\n"
"mNNJAgMBAAGjYDBeMB8GA1UdIwQYMBaAFEFsszFtm9zqgalzh3sFDe97YKeXMB0G\n"
"A1UdDgQWBBRMvSiQiFQMEgMzRvkKzvrOYLSPYDAMBgNVHRMBAf8EAjAAMA4GA1Ud\n"
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEADZ635KFi0+YybqkKfe1iEfyx\n"
"5vjWwdt+/uUZEWZL4htxOjt9sqq5agWgapjzbcwH9JyKbPph60ihM0JAvdktur+l\n"
"111LDQgD9qpkqX6Nk5i7BdDTPSjTJ/+b5c63WPfFZLlt078BUnC76uMrEICt2487\n"
"p1qaHhha0CFlUD79J1FG9HZiocUGH4KBUC41a7x77kPNzXRitCsBRxM1FnPjfQQ2\n"
"KOpuz9UWVOGeZ73/fCRFv31MOKekCW2XpWUkwNqvDtm+wQhnYfcHXPWHtMknonpM\n"
"fdedoZ1kRXX7JXs1tLZ/h7CIoTkxzsOCde4fz+bbvzFtHcdjJCYraIT221iP/g==\n"
"-----END CERTIFICATE-----";

/*
 * PEM-encoded client private key.
 *
 * Must include the PEM header and footer:
 * "-----BEGIN RSA PRIVATE KEY-----"
 * "...base64 data..."
 * "-----END RSA PRIVATE KEY-----";
 */
static const char clientcredentialCLIENT_PRIVATE_KEY_PEM[] =
"-----BEGIN RSA PRIVATE KEY-----\n"
"MIIEowIBAAKCAQEA1LE+uhTLkacgo+EgPvVyjzEgB9mAvY0r3fS6+Sm+eEoSLC/u\n"
"YooKLny0jb3M1ks3R2qF5J9AMau3YhcYa2j1TN+V08CVyCjMKp4SW3IRxvkkX+JC\n"
"9i/WabqF2knNZWpk1lHGusUIOPXlJNoNhBJ8bQZGXZPhTfTt+zYHwOqN1SQU1t81\n"
"m1oiz2rar0Q2Sw8pMpk1ChBigE+tqaN8oARI4BZqKJUqMPPOsq3mI4/eeLgVblun\n"
"86XOJUOVHp2BKQ0Hwv6ZylsjRL1iNKUjhLuXaf+wdw6OJLY6Xkp5vb7QNe9OENr0\n"
"LMhAgGrsJjgbz38tSrIBRH7f5AMPeSCs4ZjTSQIDAQABAoIBADHYhCnZ+8db0qN1\n"
"fDglD78NNUOBrtE9h31SAIuqQ9AvE8k1RnnjffVdFndtdtb187OR/GUTCVlas/SS\n"
"ExABHhPxBk0FAlVRcl++gfDab94gv2yVOzo1BdvrcbkxuKg+4fj0W02kJYQr5JLZ\n"
"Y3eDWZO3d8AwiwnZD/jitbh5f9nmWAo19sUOKuEz0JoJT91Dfwi1Lqy4m7z7hGsE\n"
"Oin4gmKdLQ8Xqr6E5Rl4IfLvqlcnyrNWY6IGh7ZBWGryJL9+DRPfNYK2Z+f8qO0V\n"
"FSywE1z3GefPdaJi69c6Bfj8XIr3v3eZGhBwcWgGA/26bka7En++/PHhwU8OaM/O\n"
"qLDJqO0CgYEA/v8mef6YNLn+Mqrfyt0v2vdE2uhzVnkVHEOFJXkhfWoZamrmSwuw\n"
"j7XWFMEti0cAgzjZIESTRGEQuOSXEJfyijbJGKHKtY+vwXp60aD7n9GW2rmgBVbO\n"
"5BDbnImFhC06vzYChdNaKlOzTCW6lSYWg5ceX8P/xkvetS8zeMjsPscCgYEA1Yd7\n"
"pVgJlfSCvo3vnRstdKoIeV5HhGBQS8kUtBaGDKm/u9lF1Y75v2PURwHq5VtRTNoz\n"
"00xuEK3DTvZtlbmtFyiyn3p4jM1+0esEsky8YfwkP9Ckw4tF/U+V4UVRwacgn93r\n"
"Y05neNtOEWFW8Kp8hus9JaVjINkRTbv+Kbi6jW8CgYEA8QpDVsJhIkZGkaKQlXTs\n"
"DbH7dmqQzEb8u5DfxYJxM9odWEjglEPijJY4XboV0Tg2tdQA+s2X3+VZJ019twPL\n"
"EveitBQMSAAhBWU/v/TKplI6pVsedq2OOGvvDFO60CzpAWKOk+4rpyMjs3FepCoy\n"
"0gUBg0kE+G8zgFWz9BMJrnUCgYBrC+PRzWPo2E6fZxqj4VcVcJSU+83QsFQw2hrV\n"
"+aC0QnlLnC4lVOshptNp0a1H1FLau11V5bAcDnCmamYftJjwzkdWKhi7Rd35MOAG\n"
"3u9mE/i8QUYIfg7VYguahpzo/3ccji2OeGTpMtBdaDYkquOI4++gqtwaQJgrJz7Q\n"
"/boQYwKBgAlLpoh5YaSbsocogmvX3+UBFpprTH4NDzJqj5ZnTahhAwX0z6l79YmO\n"
"fx+J/1BwrXYABeQPEvAKGQ/f76bM5a/zOV+xuBjA1zI2pz98IGDRrOl5H0h9eGzK\n"
"6bxj3NWSi7AS/Mkeyk8Iiwuv2dLbBS83H/VD3q5ooL4lZg7GYHYv\n"
"-----END RSA PRIVATE KEY-----";

/*
 * PEM-encoded Just-in-Time Registration (JITR) certificate (optional).
 *
 * If used, must include the PEM header and footer:
 * "-----BEGIN CERTIFICATE-----"
 * "...base64 data..."
 * "-----END CERTIFICATE-----";
 */
static const char * clientcredentialJITR_DEVICE_CERTIFICATE_AUTHORITY_PEM = NULL;