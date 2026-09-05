# 빌드 명령

프로젝트 루트:
`/Users/leejj/Desktop/workspace/UEWorkspace/SkillMaker`

엔진 경로:
`/Users/Shared/Epic Games/UE_5.8`

## 프로젝트 파일 생성
```sh
"/Users/Shared/Epic Games/UE_5.8/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh" \
  -project="/Users/leejj/Desktop/workspace/UEWorkspace/SkillMaker/SkillMaker.uproject" \
  -game -engine
```

## 에디터 타깃 빌드
```sh
"/Users/Shared/Epic Games/UE_5.8/Engine/Build/BatchFiles/Mac/Build.sh" \
  SkillMakerEditor Mac Development \
  -Project="/Users/leejj/Desktop/workspace/UEWorkspace/SkillMaker/SkillMaker.uproject"
```

## 게임 타깃 빌드
```sh
"/Users/Shared/Epic Games/UE_5.8/Engine/Build/BatchFiles/Mac/Build.sh" \
  SkillMaker Mac Development \
  -Project="/Users/leejj/Desktop/workspace/UEWorkspace/SkillMaker/SkillMaker.uproject"
```

## 에디터 실행
```sh
"/Users/Shared/Epic Games/UE_5.8/Engine/Binaries/Mac/UnrealEditor.app/Contents/MacOS/UnrealEditor" \
  "/Users/leejj/Desktop/workspace/UEWorkspace/SkillMaker/SkillMaker.uproject"
```

## 에디터 명령줄 실행·헤드리스 기동 확인
```sh
"/Users/Shared/Epic Games/UE_5.8/Engine/Binaries/Mac/UnrealEditor-Cmd" \
  "/Users/leejj/Desktop/workspace/UEWorkspace/SkillMaker/SkillMaker.uproject" \
  -unattended -nop4 -NullRHI
```

## Mac 빌드 패키징
```sh
"/Users/Shared/Epic Games/UE_5.8/Engine/Build/BatchFiles/RunUAT.sh" BuildCookRun \
  -project="/Users/leejj/Desktop/workspace/UEWorkspace/SkillMaker/SkillMaker.uproject" \
  -platform=Mac -clientconfig=Development \
  -build -cook -stage -pak -archive \
  -archivedirectory="/Users/leejj/Desktop/workspace/UEWorkspace/SkillMaker/Build/Archive"
```

## 생성된 산출물 정리
생성 상태를 명시적으로 초기화할 때만 다음 폴더를 삭제한다:
- `Binaries/`
- `Intermediate/`
- `Saved/`
- `DerivedDataCache/`

일반적인 빌드 검증 과정에서 소스·설정·콘텐츠·사용자 IDE 설정을 삭제하지 않는다.
