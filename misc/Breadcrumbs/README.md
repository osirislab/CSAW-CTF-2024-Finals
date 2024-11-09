# Challenge

<details> 
  <summary>Click to reveal flag</summary>
   csawctf{blacklotus}
</details>

## Description

You are an investigator who has been on the trail of a wily threat actor for some time. While monitoring hacking forums, you notice they now claim to have developed a new fully undetectable remote access trojan. Curious about the truth of this claim, you decide to follow the actor’s challenge, testing your OSINT skills, knowledge of malware families and ability to separate “false flags” from the truth.

- SHA256 hash of artifact 1: `db7fde09e54f2028a15112e2af3c8a1382f73d5081de2fad3470b2e2b4267808`
- SHA256 hash of artifact 2: `648b0548a20929ce578f35165032b7fee0f5b95e79f4eb56d7f00680e7c0519e` 
- SHA256 hash of portable executable (.exe): `fd89b1a215ec3bdaeb410b4e6cae6aacfe275803593ad0bc9d13d53d2d65aefc` 

Flag format: csawctf{malwarefamily} where malwarefamily is the name of the malware.


## Solution
 <details> 
  <summary>Click to reveal solution</summary>

### Initial Clue from Screenshot:

The screenshot gives a hint towards using PNG images, even though there is no visible image in the screenshot itself. This is likely an important clue, especially in the context of an OSINT (Open Source Intelligence) challenge.

### Identify Username

On the left side of the screenshot, there's a number that likely represents a username. This could be a key piece of information.

### Conduct Initial Search Using OSINT Tools:

Use an OSINT tool like osint.rocks to perform a search using the identified username.

### Examine Profile Results:

The search results yield several profiles. Start by reviewing the most popular profiles.

### Find Clue on Reddit:

While browsing these profiles, we find a relevant post on Reddit (https://www.reddit.com/user/578139/) that provides a link to a file-sharing platform (https://www.sendspace.com/file/vwt00f). The file contains an image of a pizza.
The post hints that there might be something important hidden within the image or elsewhere.

### Look for Further Clues on Reddit:

Investigate the Reddit user's profile further. You find a link to X (formerly Twitter), but the user doesn't appear to exist there.

### Interpret Reddit Comment:

A comment in the Reddit post mentions, “Don’t forget to add the six digits.” This suggests that the six digits may be a part of a Twitter username, but it could also imply something else, like a code.

### Search for the Full Username:

Combine the Reddit hint (six digits) with the previous username to form a complete username: https://x.com/BFGREgvi578139.
Searching this yields a profile on X.

### Examine X Profile:

While the profile exists, the posts don’t reveal anything particularly useful. The profile description does mention "social coder," which may imply a presence on other platforms.

### Search for GitHub Profile:

Based on the "social coder" clue, search GitHub for a matching profile.
After several attempts, you find the profile at https://github.com/BFGREgvi578139.

### Inspect GitHub Repository:

The GitHub account contains a single repository named “food.”
Within this repository, you find the same pizza image that was linked earlier, but the SHA256 hashes of this image and the one from SendSpace differ.
This difference suggests that steganography may have been applied to the image, hinting that there could be hidden data or information embedded within it.

### Extract Data Using Steghide:

Launch Kali and use the steghide tool to extract potential hidden data from the images.
Command:

```bash
    steghide extract –sf image1.jpeg
    steghide extract –sf image2.jpeg
```

### Obtain Two Parts:

After running the extraction commands, two files are retrieved, named `part1` and `part2`. These files are likely parts of a larger hidden object.

### Join the Parts Together:

Since the files are named explicitly as part1 and part2, combine them to reconstruct the original data:

```bash
    cat part1 part2 > binary
```

### Obtain and analyze a PE32 File:

The combined file, binary, is a PE32 (Portable Executable) file. This suggests it's a Windows executable, potentially malware or a payload. Start static analysis on the PE file, checking for metadata or any relevant details. No significant metadata is found, so proceed with further analysis. Run the strings command on the PE file to search for readable text or embedded information:

```bash
    strings binary
```
<br>

The strings output reveals several important pieces of information:

#### User-Agent String:
`HTTP_USER_AGENT "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; NzT)"`
This could indicate the tool or environment used for the attack.
#### IP Address:
 This IP address `219[.]90.112.203` could be tied to the attacker’s infrastructure.

#### Potential Command or Data Format:
This `type=%d&guid=%s&os=%d&arch=%d&username=%s` suggests a template for a request or data format that might be used in the malware's operation.
        
#### File Path:
This path `C:\Users/jose/proyectos/ransomware\update.pdb` indicates the malware is working within a "ransomware" project folder.

#### Function Call:
The function `GetUserGeoID` is related to gathering geographic information about the user.

#### Ransomware Message:
This is a ransom message `Oooops, your files have been encrypted!`, clearly hinting the file is part of a ransomware payload.

#### Identify Suspicious Geographical Data:
Several unusual strings are identified, indicating locations, possibly related to the malware’s target regions:
```bash
        RU exit
        UKR exit
        MD exit
        RO exit
        BY exit
        AM exit
        KZ exit
```

These could represent exit conditions, geographical regions tied to the attack or to data exfiltration points.

## Steps to Investigate the Threat Actor

### Step 1: Analyze the IP Address
- **IP Address**: 219[.]90.112.203
- We start by examining the provided IP address. After checking AbuseIPDB and other threat intelligence databases, we find no relevant information.
- A **Google search** leads us to a **Mandiant report** that links this IP address to the **Poison Ivy** malware family.
- The report notes that attributing this activity to a specific threat actor is challenging, suggesting the IP could be used in multiple operations or by different actors.

### Step 2: Investigate the User Agent String
- **User Agent String**: `"Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; NzT)"`
- The string contains a small segment, `NzT`, which seems to refer to a program version. A search for this string yields no immediate results.
- We then focus on another part of the string, which looks like an **HTTP request format**:  
  `type=%d&guid=%s&os=%d&arch=%d&username=%s`.
- This leads us to discover a connection to **Latrodectus** and **IcedID**, two pieces of malware associated with the **LUNAR SPIDER** threat actor.

### Step 3: Examine the PDB File
- The **PDB file** path includes `C:\Users/jose/proyectos\ransomware\update.pdb`, which seems unusual.
- The file may indicate a **Spanish-speaking threat actor**, possibly pointing to the region of origin or a cultural connection.
- We keep this in mind for future analysis but don’t draw any conclusions yet.

### Step 4: Analyze “Exit” Strings
- During analysis, we find several "exit" strings in the data:
  - `RU exit`
  - `UKR exit`
  - `MD exit`
  - `RO exit`
  - `BY exit`
  - `AM exit`
  - `KZ exit`
- These strings could be related to a **geolocation-based activation or deactivation** strategy for the malware, but the context is too vague to draw definitive conclusions.

### Step 5: Check Compilation Date
- We discover that the **executable** was compiled on **July 12, 2023, at 16:14 UTC**.
- This date is significant because, despite being over a year old, a forum post claims the file to be a “new FUD RAT.”
- The **discrepancy** between the compilation date and the forum post raises suspicions and may indicate the file is not as "new" as claimed, possibly suggesting an older, repurposed tool.

### Step 6: Explore Social Media Clue
- A reference on **X.com** (formerly Twitter) describes the threat actor as a **“social coder”**.
- This could be a clue leading to more information about the actor or their tools. We decide to **investigate GitHub** next, as it may hold useful details about the actor's development or related projects.

This gives pretty good matches towards BlackLotus. We try to dig into the repository and we eventually find also `type=%d&guid=%s&os=%d&arch=%d&username=%s`. 

While researching **BlackLotus**, we discover that it employs a geographic-based deactivation mechanism, closely resembling what we observe in the executable we’re analyzing. 

Based on this information, we find significant similarities with **BlackLotus**. Online reports from ESET reveal that it was initially sold on the black market as a UEFI bootkit. We then proceed to construct the flag following the specified format. 


</details>
