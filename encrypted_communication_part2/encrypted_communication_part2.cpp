// ---------------------------------------------------
// Names : Charles Ancheta, Poulomi Ganguly
// ID: 1581672 (Charles)
// CMPUT 274 , Fall 2019
//
// Assignment 2: Encrypted Arduino Communication Part 2
// ---------------------------------------------------

#include <Arduino.h>

// defines Arduino states for key exchange
enum States
{
  Start,
  WaitForKey,
  WaitForAck,
  DataExchange,
  Timeout,
};

void setup() {
  init();
  Serial.begin(9600);
  Serial3.begin(9600);
  // digital pin to determine Arduino's role (client or server)
  pinMode(13, INPUT);
  // analog pin to generate random numbers
  pinMode(A1, INPUT);
}
// generates a random number with given number of bits
uint32_t random_num(uint8_t k) {
  uint32_t num = 0;
  for (uint32_t i = 0; i < k; i++) {
    // reads LSB of A1
    uint8_t bit = analogRead(A1) & 1;
    num <<= 1;
    num |= bit;
  }
  return num;
}
// returns true if the number is prime
bool prime_check(uint32_t n) {
  if (n <= 1) {
    return false;
  }
  if (n <= 3) {
    return true;
  }
  if (n % 2 == 0 || n % 3 == 0) {
    return false;
  }
  for (int i = 5; i * i <= n; i += 6) {
    if (n % i == 0 || n % (i + 2) == 0) {
      return false;
    }
  }
  return true;
}
// generates a random prime number within the range [2^k-1,2^k)
uint32_t random_prime(uint8_t k) {
  uint32_t min = (uint32_t)1 << (k - 1);
  uint32_t max = (uint32_t)1 << k;
  uint32_t num = min + random_num(k - 1);
  // increments by 2 or 1 until it becomes prime
  while (!prime_check(num)) {
    // skips along odd numbers only
    if (num & 1) {
      num += 2;
    } else {
      num += 1;
    }
    // wraps around if number exceeds the given range
    if (num >= max) {
      num = min;
    }
  }
  return num;
}
// fast gcd function posted in eclass
uint32_t gcd(uint32_t a, uint32_t b) {
  while (b > 0) {
    uint32_t tmp = a % b;
    a = b;
    b = tmp;
  }
  return a;
}

// finds the modular multiplicative inverse of e
uint32_t mod_inverse(uint32_t e, uint32_t tot) {
  // function derived from EEA worksheet
  int32_t d = 1, y = 0;
  uint32_t t = tot;
  // similar to gcd function
  while (t > 0) {
    uint32_t q = e / t;
    int32_t tmp1 = d - (q * y);
    d = y;
    y = tmp1;
    uint32_t tmp2 = e % t;
    e = t;
    t = tmp2;
  }
  // ensures that x >= 0 and x < tot
  while (d < 0) {
    d += tot;
  }
  return d %= tot;
}
// generates the Arduino's public and private keys and modulus
void key_gen(uint32_t &d, uint32_t &n, uint32_t &e) {
  uint32_t p = random_prime(15), q = random_prime(16);
  n = p * q; // Arduino's modulus
  uint32_t tot = (p - 1) * (q - 1);
  e = random_prime(15); // Arduino's public key
  uint32_t f = (1 << 15);
  while (gcd(e, tot) != 1) {
    e += 1;
    // wraps around to ensure that e stays a 15-bit int
    if (e >= f) {
      e = (1 << 14);
    }
  }
  d = mod_inverse(e, tot); // Arduino's private key
}
// performs modular multiplication on two factors
uint32_t mulmod(uint32_t a, uint32_t b, uint32_t m) {
  uint32_t res = 0, term = b % m;
  while (a > 0) {
    // adds the binary expansion term of the current
    // bit on the loop if the bit is 1 (2^i * b)
    if (a & 1) {
      res = (res + term) % m;
    }
    term = (term << 1) % m;
    a >>= 1;
  }
  return res;
}
// performs modular exponentiation
uint32_t powmod(uint32_t x, uint32_t pow, uint32_t m) {
  uint32_t ans = 1;
  // modulo before multiplying to avoid overflow
  uint32_t pow_x = x % m;
  while (pow > 0) {
    if (pow & 1) {
      ans = mulmod(ans, pow_x, m);
    }
    // pow_x is squared if exponent is even
    // to reduce the number of operations
    pow_x = mulmod(pow_x, pow_x, m);
    pow >>= 1;
  }
  return ans % m;
}
// given functions to send and receive uint32
void uint32_to_serial3(uint32_t num) {
  Serial3.write((char)(num >> 0));
  Serial3.write((char)(num >> 8));
  Serial3.write((char)(num >> 16));
  Serial3.write((char)(num >> 24));
}
uint32_t uint32_from_serial3() {
  uint32_t num = 0;
  num = num | ((uint32_t)Serial3.read()) << 0;
  num = num | ((uint32_t)Serial3.read()) << 8;
  num = num | ((uint32_t)Serial3.read()) << 16;
  num = num | ((uint32_t)Serial3.read()) << 24;
  return num;
}
// gets char from Serial and sends it to Serial 3
void send_char(uint32_t e, uint32_t m) {
  uint32_t c = Serial.read() - '\0';
  // converts to encrypted uint32 and sends to Serial3
  uint32_t x1 = powmod(c, e, m);
  uint32_to_serial3(x1);
  // also writes char on own Serial
  Serial.write(c);
  if (c == 13) {
    Serial.write('\n'); // return key special case
  }
  if (c == 8) {
    Serial.print(" \b"); // backspace key special case
  }
}
// decrypts the uint32 from Serial 3 and writes it on Serial
void receive_char(uint32_t x, uint32_t d, uint32_t n) {
  uint32_t y = powmod(x, d, n);
  Serial.write(y);
  if (y == 13) {
    Serial.write('\n'); // return key special case
  }
  if (y == 8) {
    Serial.write(" \b"); // backspace key special case
  }
  // clears garbage bytes so that restarting wouldn't be necessary
  // in case the buffer overflows
  if (y > 255) {
  	Serial.print("Overflow! Please wait...  ");
    while(Serial3.available() > 0) {
      Serial3.read();
    }
    Serial.println("Proceed.");	
  }
  
}
// given function for finite state machines
bool wait_on_serial3(uint8_t nbytes, long timeout) {
  unsigned long deadline = millis() + timeout; // wraparound not a problem
  while (Serial3.available() < nbytes && (timeout < 0 || millis() < deadline)) {
    delay(1); // be nice, no busy loop
  }
  return Serial3.available() >= nbytes;
}
// follows the server state diagram of key exchange
void server_exchange(uint32_t n, uint32_t &e, uint32_t &m) {
  States state = Start;
  bool sentACK;
  bool first_listen = true;
  uint32_t ckey, cmod;
  while (state != DataExchange) {
    if (state == Timeout) {
      Serial.println("Timeout!");
      state = Start;
      first_listen = true;
    }
    if (state == Start) {
      if (first_listen) {
        Serial.print("Listening for CR... ");
        first_listen = false;
      }
      sentACK = false;
      if (!wait_on_serial3(1, 1000) || Serial3.read() != 'C') {
        state = Timeout;
      } else {
        state = WaitForKey;
      }
    }
    if (state == WaitForKey) {
      if (!wait_on_serial3(8, 1000)) {
        state = Timeout;
      } else {
        // stores client keys and sends ACK
        Serial.print("Keys received! ");
        ckey = uint32_from_serial3();
        cmod = uint32_from_serial3();
        if (!sentACK) {
          Serial3.write('A');
          uint32_to_serial3(e);
          uint32_to_serial3(n);
          sentACK = true;
        }
        state = WaitForAck;
      }
    }
    if (state == WaitForAck) {
      if (!wait_on_serial3(1, 1000)) {
        state = Timeout;
      } else {
        // checks whether key exchange is complete
        char recv = Serial3.read();
        if (recv == 'A') {
          state = DataExchange;
          e = ckey;      
          m = cmod;
        } else if (recv == 'C') {
          state = WaitForKey; // consumes all the CRs sent
        }
      }
    }
  }
}
// follows the client state diagram of key exchange
void client_exchange(uint32_t n, uint32_t &e, uint32_t &m) {
  States state = Start;
  uint32_t skey, smod;
  while (state != DataExchange) {
    if (state == Timeout) {
      Serial.println("Timeout!");
      state = Start;
    }
    if (state == Start) {
      Serial.print("Sending CR... ");
      Serial3.write('C');
      uint32_to_serial3(e);
      uint32_to_serial3(n);
      state = WaitForAck;
    }
    if (state == WaitForAck) {
      // waits for ACK (9 bytes: 'A', skey, smod)
      if (!wait_on_serial3(9, 1000) || Serial3.read() != 'A') {
        state = Timeout;
      } else {
        skey = uint32_from_serial3();
        smod = uint32_from_serial3();

        Serial.print("Keys Received! ");
        Serial.println("Sending ACK...");
        state = DataExchange;
        e = skey;
        m = smod;
        Serial3.write('A');
      }
    }
  }
}
// exchanges and displays keys with other Arduino
void key_exchange(uint32_t &e, uint32_t n,uint32_t &m) {
  // shows Arduino's keys
  Serial.print("My Public Key: ");
  Serial.print(e);
  Serial.print("  My Modulus: ");
  Serial.println(n);
  if (digitalRead(13) == LOW) {
    client_exchange(n, e, m);
  } else {
    server_exchange(n, e, m);
  }
  // shows other Arduino's keys for confirmation
  Serial.print("Their Public Key: ");
  Serial.print(e);
  Serial.print("  Their Modulus: ");
  Serial.println(m);
  // clears any garbage bytes before starting chat
  while (Serial3.available() > 0) {
    Serial3.read();
  }
  while (Serial.available() > 0) {
    Serial.read();
  }
}
int main() {
  uint32_t d, n, e, m;
  setup();
  key_gen(d, n, e);
  Serial.println("Key Generation Complete!");
  key_exchange(e, n, m);
  Serial.println("Chat ready...");

  while (true) {
    // reads char (1 byte)
    if (Serial.available() > 0) {
      send_char(e, m);
    }
    // receives uint32 (4 bytes)
    if (Serial3.available() >= 4) {
      receive_char(uint32_from_serial3(), d, n);
    }
  }
  Serial.flush();
  Serial3.flush();
  return 0;
}