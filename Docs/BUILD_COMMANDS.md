# Build Commands

Project root:
`/Users/leejj/Desktop/workspace/UEWorkspace/SkillMaker`

Engine:
`/Users/Shared/Epic Games/UE_5.8`

## Generate Project Files
```sh
"/Users/Shared/Epic Games/UE_5.8/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh" \
  -project="/Users/leejj/Desktop/workspace/UEWorkspace/SkillMaker/SkillMaker.uproject" \
  -game -engine
```

## Build Editor Target
```sh
"/Users/Shared/Epic Games/UE_5.8/Engine/Build/BatchFiles/Mac/Build.sh" \
  SkillMakerEditor Mac Development \
  -Project="/Users/leejj/Desktop/workspace/UEWorkspace/SkillMaker/SkillMaker.uproject"
```

## Build Game Target
```sh
"/Users/Shared/Epic Games/UE_5.8/Engine/Build/BatchFiles/Mac/Build.sh" \
  SkillMaker Mac Development \
  -Project="/Users/leejj/Desktop/workspace/UEWorkspace/SkillMaker/SkillMaker.uproject"
```

## Open Editor
```sh
"/Users/Shared/Epic Games/UE_5.8/Engine/Binaries/Mac/UnrealEditor.app/Contents/MacOS/UnrealEditor" \
  "/Users/leejj/Desktop/workspace/UEWorkspace/SkillMaker/SkillMaker.uproject"
```

## Run Editor Commandlet/Headless Smoke Check
```sh
"/Users/Shared/Epic Games/UE_5.8/Engine/Binaries/Mac/UnrealEditor-Cmd" \
  "/Users/leejj/Desktop/workspace/UEWorkspace/SkillMaker/SkillMaker.uproject" \
  -unattended -nop4 -NullRHI
```

## Package Mac Build
```sh
"/Users/Shared/Epic Games/UE_5.8/Engine/Build/BatchFiles/RunUAT.sh" BuildCookRun \
  -project="/Users/leejj/Desktop/workspace/UEWorkspace/SkillMaker/SkillMaker.uproject" \
  -platform=Mac -clientconfig=Development \
  -build -cook -stage -pak -archive \
  -archivedirectory="/Users/leejj/Desktop/workspace/UEWorkspace/SkillMaker/Build/Archive"
```

## Clean Generated Outputs
Delete only when intentionally refreshing generated state:
- `Binaries/`
- `Intermediate/`
- `Saved/`
- `DerivedDataCache/`

Do not delete source, config, content, or user IDE settings as part of routine build validation.
