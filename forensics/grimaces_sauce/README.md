# Grimaces Sauce

<details> 
  <summary>Click to reveal flag</summary>
  
   ```
   csawctf{f0rensIcZ_I3_n0T_sAucY!}
   ```
</details>

## Tools & Plugins

* Volatility 2 & Volatility 3
* Plugins to read notepad app; view chrome history

## Description

McDonald's themed  forensic challenge to try to decrypt the secret sauce recipe. 

## Solution
 <details> 
  <summary>Click to reveal solution</summary>

#### DESC
```
Grimace! Hamburglar has just corrupted my laptop...
but not before I was able to exfiltrate the secret sauce recipe! Keep it safe! 

Sincerly, Ronald
```

#### FILES
* **grimace.7z** 
* **secret_sauce_recipe_from_ronald.gpg**
```
-----BEGIN PGP MESSAGE-----

wYwDbdyQ9GUtEioBA/sH8PGIrQ0DEpMEmjvJNyPEqcuMiYD84VXnS9vre2R+CvXQ
62fgqdMkwZ9wwR3aXRp6oD7rZawrnZFwJQ3Rpdlf7r+AM9Na8ShAFE8pC2cTkvvc
v80UyQDOMOSj48EdGQxk1USifmKQcXfHB4ML7LeDX+tMIfPySJ4B7T/D8g3D2tLA
SAFRdYuJc0D9ZyCHgIc5oMOZx3fdAEWitTP9g5MIxkwpqeFrugbDRcWUJkSDsE3D
VBq2NkG37Sa+EKQPggAPIugt2JZmJNm2KiwUmrrY5G/wW6V4UmD8LxKcZpXR1g24
4bYfCPa7HPbHNDTBJYoWk6VkBKAH07a1lP4UwCPZw1So+YRAxxxHWrt5xE297aaR
5ndUImIeFHW8E4fFqj/ccvZQKuIW8SIVZd/CKHr+Z1sh8ntm8YGpboYCO54s5E/d
ZdMywP1WOrMD5qwaH0LegRkHckPcberDb5BKYlCDrro2HYbklDg7XOaWpvJOdxUN
C0DIKVAAZ90EBRiXJ2AlPkJKCDdiu+FJgg==
=VeFh
-----END PGP MESSAGE-----
```

#### **ID OS details**
```
python2 ~/coding_software/volatility/vol.py -f memory.dmp volatility  --profile=Win10x64_17134   imageinfo
```

#### **View unsaved notepad contents**
*drafting message to his confidant Ronald McDonald it happens to contain his private gpg key*

```
python3 ~/coding_software/volatility3/vol.py -f memory.dmp  notepad  | grep "END PGP PRIVATE KEY BLOCK" 
```

![image](https://github.com/user-attachments/assets/db496f10-9041-41e4-a23a-5ab7bd0df0ed)

_Note you have the private key here, which we can save as `mykey.asc`_



#### **GPG commands to reveal message**
```
gpg --import mykey.asc
gpg --output dec.txt --decrypt secret_sauce_recipe_from_ronald.gpg
```

![image](https://github.com/user-attachments/assets/1e1584d8-d303-477b-8942-cdcdebeb16dc)



#### **SEARCH FOR FILE WITH GOOD IN NAME**
```
python3 ~/coding_software/volatility3/vol.py -f memory.dmp filescan | grep -i good
```

![image](https://github.com/user-attachments/assets/63150de0-59d3-4400-b033-331142a76934)

_Note the offsets_


#### **Dump the file with the offset**
```
python3 ~/coding_software/volatility3/vol.py -f memory.dmp -o "HERE/" windows.dumpfiles --virtaddr 0x808e44c47380
```

![image](https://github.com/user-attachments/assets/202b6fb3-c803-417e-b66c-351846f41f6f)


_Note the file is password protected_

##### Envars
Search envars to reveal environemnt variable: `UNLOCK_HAPPY_MEAL`:`billions_and_billions`
```
python3 ~/coding_software/volatility3/vol.py -f memory.dmp windows.envars
```

#### **CHECK the browswer history given  the hint**

```
python2 ~/coding_software/volatility/vol.py -f memory.dmp volatility  --profile=Win10x64_17134  chromehistory 
```

_Here you'll find a link to a recently visted `secret_sauce_decrypter.s` file in assembly_


#### Decrypt the sauce

As you have the recipe (from the GPG message) and the key (revealed through steghide) compile the assembly to reveal the flag

```
gcc -c secret_sauce_decrypter.s && gcc secret_sauce_decrypter.s && ./a.out
```
![image](https://github.com/user-attachments/assets/e4fd10cf-0830-43b7-ba25-f6105fc83026)

</details>
