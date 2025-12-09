# PasswordsGenerator

[![Build Status](https://github.com/unstable-code/PasswordsGenerator/actions/workflows/build.yml/badge.svg)](https://github.com/unstable-code/PasswordsGenerator/actions/workflows/build.yml)
[![Release](https://img.shields.io/github/v/release/unstable-code/PasswordsGenerator)](https://github.com/unstable-code/PasswordsGenerator/releases/latest)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-blue)](#설치-방법)
[![Language](https://img.shields.io/badge/language-C%2B%2B17-00599C?logo=cplusplus)](CMakeLists.txt)
[![Qt](https://img.shields.io/badge/Qt-6.5%2B-41CD52?logo=qt)](https://www.qt.io/)

### 제작 동기 및 사용방법
- 회원 가입시 매우 다양한 비밀번호 규칙으로 인해 비밀번호를 정하는데 시간이 오래걸려 이 프로그램을 제작하게 되었습니다.
- 하단 프롬프트에 `help` 를 입력하면 사용가능한 명령을 표시해 줍니다.
- 여담으로 아이디도 이 프로그램을 통해 생성이 가능합니다.

### 요구사항
- CMake 3.16 이상
- Qt 6.5 이상
- C++17 지원 컴파일러

### 설치 방법
**C++/Qt6로 재구현된 크로스 플랫폼 버전**이 제공됩니다. 아래 플랫폼에서 사전 빌드된 바이너리를 다운로드할 수 있습니다:

- **Windows** (x64) - [다운로드](../../releases/latest) - 단일 실행 파일 제공 (정적 링킹)
- **Linux** (x64) - [다운로드](../../releases/latest)
- **macOS** (x64) - [다운로드](../../releases/latest) - .app 번들 제공

#### 첫 실행 시 보안 경고 해결 방법

이 앱은 코드 사이닝이 되어있지 않아 첫 실행 시 보안 경고가 표시될 수 있습니다.

**Windows (SmartScreen 경고):**
1. `Windows의 PC 보호` 창이 표시되면 `추가 정보` 클릭
2. `실행` 버튼을 클릭하여 실행

**macOS (Gatekeeper 경고):**
1. 앱을 실행하면 "손상되어 열 수 없습니다" 경고가 표시될 수 있습니다
2. `시스템 설정` > `개인 정보 보호 및 보안` > `보안` 섹션으로 이동
3. `확인 없이 열기` 버튼을 클릭
4. 또는 터미널에서 다음 명령 실행:
   ```bash
   xattr -cr /Applications/PasswordGenerator.app
   ```

### 빌드 방법
```bash
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

### Demo
<img width="1171" height="612" alt="image" src="https://github.com/user-attachments/assets/a029abf0-92bc-4855-bc04-e431e9923e0e" />

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

