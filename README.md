# PasswordsGenerator
[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://paypal.me/hmDonate)
### 제작 동기 및 사용방법
- 회원 가입시 매우 다양한 비밀번호 규칙으로 인해 비밀번호를 정하는데 시간이 오래걸려 이 프로그램을 제작하게 되었습니다.
- 하단 프롬프트에 `help` 를 입력하면 사용가능한 명령을 표시해 줍니다.
- 여담으로 아이디도 이 프로그램을 통해 생성이 가능합니다.

### 요구사항
- 프로젝트를 빌드하기 위해 JDK 또는 OpenJDK가 필요합니다.
- 이 프로그램은 GUI를 사용하므로 CLI기반 시스템에서는 빌드할 수 없습니다.

### 설치 방법
- **macOS 및 Linux**
  - [compileProject.sh](https://github.com/unstable-code/PasswordsGenerator/blob/master/compileProject.sh) 스크립트를 사용해서 java 파일을 빌드합니다.
  - 또는 아래의 명령어를 사용해서 java 파일을 직접 빌드합니다.
    ``` bash
    javac PasswdGen.java
    jar -cmf Manifest.mf passwdGen.jar *.class
    chmod 755 passwdGen.jar
    ```
  - 생성된 jar 파일을 더블 클릭하여 실행하거나 `java -jar passwdGen.jar` 명령어를 사용해서 상세 로그를 얻을 수 있습니다.

- **Windows**
  - [compileProject.bat](https://github.com/unstable-code/PasswordsGenerator/blob/master/compileProject.bat) 스크립트를 사용해서 java 파일을 빌드합니다.
  - 생성된 jar 파일을 '새로운 가상본 만들기' 기능을 사용하여 아래와 같이 설정한 후 사용합니다.
    ```
        Target: "%javapath%\bin\javaw.exe" -jar -Dfile.encoding=ms949 passwdGen.jar
        Start in: %path%\PasswordsGenerator\PasswordsGenerator
    ```
  - 또는 `java -jar -Dfile.encoding=ms949 passwdGen.jar` 명령어를 사용해서 상세 로그를 얻을 수 있습니다.

### 주의사항
- Windows에서는 인코딩 문제로 화면이 제대로 표시되지 않을 수 있습니다. 이 문제가 발생할 경우 상단의 제공된 스크립트를 사용하여 다시 시도해 봅시다.

### 문제 해결
- **컴파일 도구**
  - `FATAL ERROR!`: 프로젝트 구조가 손상되어 있습니다.
  - `ERROR! No GUI detected`: GUI가 감지되지 않았습니다. 현재 사용하고 계신 컴퓨터가 GUI를 지원하는지, 또 컴퓨터가 GUI를 지원하지만 시스템이 CLI 기반으로 운영되고 있거나, WSL(aka. Windows Subsystem for Linux)에서 빌드를 시도하진 않았는지 다시 한번 확인하세요.
  - `ERROR! This script only support...`: 지원하지 않는 OS이거나 컴파일 도구를 잘못 실행했습니다.
  - `ERROR! This script requires JDK`: 계속 진행하려면 JDK가 있어야 합니다. [여기](https://github.com/unstable-code/PasswordsGenerator#%EC%9A%94%EA%B5%AC%EC%82%AC%ED%95%AD)를 참조하세요.
  - `ERROR! JDK and JVM versions...`: JDK와 JVM 버전이 다릅니다. 계속하려면 두 패키지간 버전을 동일하게 설정해야 합니다.
  - `jar file couldn't be created`: 프로그램이 이미 열려있어서 쓰기 권한이 없거나 .class 파일을 찾을 수 없습니다.
- **프로그램 에러**
  - The file passwdGen.jar isn't marked as executable: `chmod 755 passwdGen.jar`를 터미널에서 실행한 후 다시 시도하세요.
  - 알아볼 수 없는 □ 문자가 표시될 경우 [여기](https://github.com/unstable-code/PasswordsGenerator#%EC%A3%BC%EC%9D%98%EC%82%AC%ED%95%AD)를 참조하세요.
  - 제보를 권장하는 예외가 발생했을 경우 [여기로 제보](https://github.com/unstable-code/PasswordsGenerator/issues)해 주세요.

### Demo
<img width="758" alt="Screen Shot 2021-11-21 at 0 05 24" src="https://user-images.githubusercontent.com/25660580/142732280-c83b466b-8764-4a0d-bde9-ffd300e81dee.png">

### 참고사항
- 이 프로그램은 아래 사항을 모두 만족해야 `규정에 부합하지 않음` 경고를 표시하지 않습니다.
  - 비밀번호 길이가 12자 이상이며, 다른 옵션이 모두 켜져있어야 함
  - 이 기준에 만족하지 않을 경우 생성할 갯수 명령에 따라 경고가 반복됩니다.
- 지우기 버튼은 적어도 한 번 이상 비밀번호를 생성해야 클립보드를 지울 수 있습니다.
  - 지우기 버튼은 버퍼도 함께 청소하며, 버퍼만 청소하고 싶을 경우 `clear` 명령을 사용하세요.
  - 단 제보를 권장하는 예외가 발생했을 경우 버퍼를 청소하면 빠른 대처가 이뤄지지 않을 수 있습니다.

### 개인정보 처리방침
- 이 프로그램은 다음 권한이 필요합니다.

|대상|권한|이유|
|:--:|:-------:|:--|
|OS|r-|GUI 감지, 웹 페이지 열기 데몬을 사용하려면 해당 권한이 필요합니다.|
|클립보드|-w|생성된 비밀번호를 복사하려면 해당 권한이 필요합니다.|
|스토리지|--|저장공간에는 접근할 수 없습니다.|
|메모리|rw|비밀번호 생성 설정을 반영하려면 해당 권한이 필요합니다.|
|인터넷|--|인터넷에는 접근할 수 없습니다.|

  - 스토리지 또는 메모리에 비밀번호를 저장하지 않으므로 동일한 비밀번호가 생성될 가능성이 있습니다.
    - 이 기능을 확인하시려면 먼저 비밀번호 길이를 1로 설정한 후, 다른 옵션을 모두 끕니다.
    - 다음으로 `count 16` 명령을 입력하여 16개의 한자리 영문자 비밀번호를 확인해 보시기 바랍니다.
    - (해당 설정은 오직 위 기능을 빨리 학인하기 위함이며, 없는 저장 기능을 켜거나 끄지 않습니다.)
  - 웹 페이지 열기 데몬은 코드에 입력된 주소가 기본 브라우저로 열립니다.

