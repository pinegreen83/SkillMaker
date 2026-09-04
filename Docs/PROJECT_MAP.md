# Project Map

## Root
- `SkillMaker.uproject`: UE 5.8 project, runtime module `SkillMaker`.
- `Source/SkillMaker.Target.cs`: game target.
- `Source/SkillMakerEditor.Target.cs`: editor target.
- `Config/DefaultEngine.ini`: default map, GameMode, redirects, renderer/platform settings.
- `Config/DefaultInput.ini`: Enhanced Input defaults.
- `Content/SkillMaker`: project-owned gameplay assets, maps, data tables, input assets, UI widgets.
- `Docs/`: project map, build commands, coding conventions, and AI work rules.

Target settings:
- `Source/SkillMaker.Target.cs`: `TargetType.Game`, `BuildSettingsVersion.V7`, `EngineIncludeOrderVersion.Unreal5_8`.
- `Source/SkillMakerEditor.Target.cs`: `TargetType.Editor`, `BuildSettingsVersion.V7`, `EngineIncludeOrderVersion.Unreal5_8`.

## Module
`Source/SkillMaker/SkillMaker.Build.cs`
- Public deps: `Core`, `CoreUObject`, `Engine`, `InputCore`, `UMG`, `Slate`, `SlateCore`, `EnhancedInput`.
- Private deps: `Niagara`.

## Gameplay Areas
- `Source/SkillMaker/Character/`
  - `ASKBaseCharacter`: shared skill/combat component owner.
  - `ASKPlayerCharacter`: movement, camera, weapon, interaction, skill use.
  - `ASKAICharacter`, `ASKPreviewCharacter`: AI and editor/preview character variants.
- `Source/SkillMaker/Player/`
  - `ASKPlayerController`: Enhanced Input, movement/look/jump/interact, Q/E/R/F skill slots.
  - `ASKSkillMakerController`: skill maker/editor controller.
- `Source/SkillMaker/Skill/`
  - `FSKSkillData`: attack skill definition data, montage, cooldown, damage, projectile, notify, range, status effects. It is still a `FTableRowBase`, but current runtime-created skill persistence is SaveGame-based.
  - `USKSkillComponent`: skill map, cooldowns, server/multicast skill execution.
  - `ASKProjectileActor`: projectile behavior.
  - `USKSkillManager`: legacy DataTable-oriented skill manager object. Current editor UI save/load path should use `USKSaveGameSubsystem`.
- `Source/SkillMaker/Combat/`
  - `USKCombatComponent`: replicated health, damage, status effects, death.
- `Source/SkillMaker/Data/`
  - `FSKWeaponData`, `FSKAnimationData`, `FSKProjectileData`: DataTable row structs.
  - `USKDataManager`: legacy placeholder. Do not use for active DataTable access.
- `Source/SkillMaker/Game/`
  - `USKDataManagerSubsystem`: `UGameInstanceSubsystem` that loads default weapon/animation/projectile DataTables and exposes list APIs to UI.
  - `USKSaveGameSubsystem`: `UGameInstanceSubsystem` that loads/creates `USKPlayerSkillSave`, saves skills to slot, and exposes SkillID-based list/get APIs.
  - `USKGameInstance`: project GameInstance class. Current `Init()` only calls `Super::Init()`.
- `Source/SkillMaker/Save/`
  - `FSKSkillSet`: `SkillID -> FSKSkillData` map container.
  - `USKPlayerSkillSave`: SaveGame object containing the current skill map and named skill sets.
- `Source/SkillMaker/GameMode/`
  - `ASKSkillMakerGameMode`: editor/skill maker flow, editor HUD.
  - `ASKSkillTrainGameMode`: training flow, player pawn/controller, training HUD.
- `Source/SkillMaker/UI/UI-SkillMaker/`
  - `UEEditor/`: skill maker editor HUD/main widget. The editor HUD creates `WBP_SKSkillMakerEditorMain` and owns `CurrentEditingSkill`.
  - `TrainingRoom/`: training room HUD/main widget.
  - `Common/`: reusable selection/detail widgets for skills, weapons, animations, projectiles, status effects.
- `Source/SkillMaker/Animation/`
  - custom skill animation notifies and player anim instance. `USKSkillAnimNotify_Trigger` is the projectile/skill effect trigger notify used by the skill editor.
- `Source/SkillMaker/Prop/`
  - interactable actors and skill station.
- `Source/SkillMaker/Logging/`
  - `LogSkillMaker` category and `SK_LOG` helper macro.

## Source File Index

### Module
- `Source/SkillMaker/SkillMaker.h/.cpp`: module entry point.
- `Source/SkillMaker/SkillMaker.Build.cs`: runtime module dependency list.

### Character
- `Character/SKBaseCharacter.h/.cpp`: base character. Owns `USKSkillComponent` and `USKCombatComponent`, stores `CurrentSkillData`, and forwards `UseSkill(SkillID)` into the skill component.
- `Character/SKPlayerCharacter.h/.cpp`: playable character. Handles movement/camera-facing helpers, weapon mesh state, and interactable target references.
- `Character/SKAICharacter.h/.cpp`: AI character variant. Currently thin over `ASKBaseCharacter`.
- `Character/SKPreviewCharacter.h/.cpp`: editor preview character variant used by the skill maker HUD.

### Player
- `Player/SKPlayerController.h/.cpp`: Enhanced Input setup, movement/look/jump/interact handling, and Q/E/R/F skill slot input dispatch.
- `Player/SKSkillMakerController.h/.cpp`: controller class for the skill maker map. Current code is minimal.

### Skill
- `Skill/SKSkillData.h`: skill enums and data structs. `FSKSkillData` remains table-row compatible and is also marked for SaveGame persistence.
- `Skill/SKSkillComponent.h/.cpp`: skill storage, cooldown checks, server RPC, multicast montage/effect execution, and preview-compatible execution through `ASKBaseCharacter`.
- `Skill/SKProjectileActor.h/.cpp`: projectile actor with movement/collision behavior and skill-derived damage/effect data.
- `Skill/SKSkillManager.h/.cpp`: legacy DataTable-based skill manager. Keep it isolated unless intentionally migrating old DataTable skill rows.

### Combat
- `Combat/SKCombatComponent.h/.cpp`: replicated health, damage application, status effect application/removal, and death handling.

### Data
- `Data/SKWeaponData.h`: legacy weapon DataTable row struct.
- `Data/SKAnimationData.h`: legacy animation DataTable row struct.
- `Data/SKProjectileData.h`: legacy projectile DataTable row struct.
- `Data/SKDataManager.h/.cpp`: legacy placeholder object. Active resource list access goes through `USKDataManagerSubsystem`.

### Game
- `Game/SKGameInstance.h/.cpp`: project GameInstance class. Current `Init()` only calls `Super::Init()`.
- `Game/SKDataManagerSubsystem.h/.cpp`: active resource DataTable subsystem. Loads default weapon/animation/projectile tables and returns UI list rows.
- `Game/SKSaveGameSubsystem.h/.cpp`: active skill persistence subsystem. Loads or creates `USKPlayerSkillSave`, saves skills by `SkillID`, and exposes saved skill list/get APIs.

### Save
- `Save/SKPlayerSkillSave.h/.cpp`: SaveGame object. `FSKSkillSet` stores a `SkillID -> FSKSkillData` map; the save object exposes set/get/list helpers.

### GameMode
- `GameMode/SKSkillMakerGameMode.h/.cpp`: skill maker map mode. Assigns skill maker controller and editor HUD classes.
- `GameMode/SKSkillTrainGameMode.h/.cpp`: training map mode. Assigns player pawn/controller and training HUD classes.

### Animation
- `Animation/SKPlayerAnimInstance.h/.cpp`: player animation instance class. Current C++ logic is minimal.
- `Animation/SKSkillAnimNotify_Trigger.h/.cpp`: named skill trigger notify. Reads `CurrentSkillData`, matches `NotifyTriggerName`, and triggers projectile/effect behavior.
- `Animation/SKSkillAnimNotify_HitCheck.h/.cpp`: melee hit-check notify class. Current implementation is incomplete and should not be treated as the main attack path for the 1차 goal.

### Prop
- `Prop/SKInteractableActor.h/.cpp`: base interactable actor with an interaction entry point.
- `Prop/SKSkillStation.h/.cpp`: interactable skill station that opens the training skill UI flow.

### UI - Editor
- `UI/UI-SkillMaker/UEEditor/SKSkillMakerEditorHUD.h/.cpp`: skill maker HUD. Creates the editor main widget, owns the current editing skill, spawns preview character, and runs preview.
- `UI/UI-SkillMaker/UEEditor/SKSkillMakerEditorMainWidget.h/.cpp`: editor flow controller. Handles new/edit state transitions, skill naming, final save, and selection widget coordination.

### UI - Training Room
- `UI/UI-SkillMaker/TrainingRoom/SKSkillMakerTrainHUD.h/.cpp`: training HUD. Creates training main widget and loads selected skill data from SaveGame.
- `UI/UI-SkillMaker/TrainingRoom/SKSkillMakerTrainMainWidget.h/.cpp`: training room widget state controller. Several handlers are still incomplete.

### UI - Common Widgets
- `Common/SkillSelect/SKSkillSelectionWidget.h/.cpp`: saved skill list UI. Current list source is `USKSaveGameSubsystem`.
- `Common/SkillSelect/SKSkillCardWidget.h/.cpp`: card for one saved skill.
- `Common/SkillSelect/SKSkillDetailWidget.h/.cpp`: detail editor for skill type, damage, range, projectile, notify, and preview trigger.
- `Common/WeaponSelect/SKWeaponSelectionWidget.h/.cpp`: weapon list UI from `USKDataManagerSubsystem`.
- `Common/WeaponSelect/SKWeaponCardWidget.h/.cpp`: card for one weapon row.
- `Common/AnimationSelect/SKAnimationSelectionWidget.h/.cpp`: animation list UI from `USKDataManagerSubsystem`, filtered by weapon type.
- `Common/AnimationSelect/SKAnimationCardWidget.h/.cpp`: card for one animation row.
- `Common/AnimationSelect/SKAnimNotifySelectionWidget.h/.cpp`: valid skill trigger notify list UI for a selected montage.
- `Common/AnimationSelect/SKAnimNotifyCardWidget.h/.cpp`: card for one notify trigger.
- `Common/ProjectileSelect/SKProjectileSelectionWidget.h/.cpp`: projectile list UI from `USKDataManagerSubsystem`; card click applies selection immediately.
- `Common/ProjectileSelect/SKProjectileCardWidget.h/.cpp`: card for one projectile row.
- `Common/StatusEffectSelect/SKStatusEffectCardWidget.h/.cpp`: card for one status effect option.

## Key Assets
- Maps:
  - `/Game/SkillMaker/Map/SkillTrainingMap`
  - `/Game/SkillMaker/Map/SkillMakingMap`
  - `/Game/SkillMaker/Map/TrainingMap`
- DataTables:
  - `DT_SkillData`
  - `DT_WeaponData`
  - `DT_AnimationData`
  - `DT_ProjectileData`
- Blueprints:
  - `BP_SKPlayerCharacter`
  - `BP_SKPreviewCharacter`
  - `BP_SKAI`
  - `BP_SKPlayerController`
  - `BP_SKSkillMakerController`
  - `BP_FireBall`
  - `BP_WaterBall`
- Input:
  - `IMC_Default`
  - `IA_Move`, `IA_Look`, `IA_Jump`
  - `IA_SkillQ`, `IA_SkillE`, `IA_SkillR`, `IA_SkillF`
- UI:
  - editor: `WBP_SKSkillMakerEditorMain`; `WBP_SKSkillMakeEditorHUD` also exists as an asset, but current C++ HUD auto-load path uses `WBP_SKSkillMakerEditorMain` directly.
  - training: `WBP_SKSkillMakerTrainMain`, `WBP_SKSkillMakerTrainHUD`
  - shared cards/selection widgets under `Content/SkillMaker/UI`.

## Current Notes
- `DefaultEngine.ini` sets `GameDefaultMap` and `EditorStartupMap` to `/Game/SkillMaker/Map/SkillTrainingMap`.
- `DefaultEngine.ini` sets `GlobalDefaultGameMode` to `/Script/SkillMaker.SkillMakerGameMode`.
- `ASKSkillTrainGameMode` assigns `BP_SKPlayerCharacter`, `BP_SKPlayerController`, and `ASKSkillMakerTrainHUD`.
- `ASKSkillMakerGameMode` assigns `BP_SKSkillMakerController` and `ASKSkillMakerEditorHUD`.
- `SkillMakingMap.umap` contains `SKSkillMakerGameMode` in its world data. Final map override status should still be verified in Unreal Editor World Settings because `.umap` is binary.
- `ASKSkillMakerEditorHUD` loads `/Game/SkillMaker/UI/Editor/WBP_SKSkillMakerEditorMain.WBP_SKSkillMakerEditorMain_C` and adds it to the viewport in `BeginPlay()`.
- `USKDataManagerSubsystem` loads default DataTables from `/Game/SkillMaker/Data/DT_WeaponData`, `/Game/SkillMaker/Data/DT_AnimationData`, and `/Game/SkillMaker/Data/DT_ProjectileData` if its table references are unset.
- Runtime-created skill save/load currently goes through `USKSaveGameSubsystem` and `USKPlayerSkillSave` with `SkillID` as the key. `DT_SkillData` may remain as seed/sample data, but it is not the current UI persistence path.
- Skill preview now assigns a temporary `PreviewSkill` ID when needed, logs selected skill data, pushes the skill into the preview character's skill map, and runs the skill through `USKSkillComponent`.
- Projectile card click immediately selects/broadcasts the projectile. The old confirm button path only logs the current selected projectile.
- AnimNotify selection lists only `USKSkillAnimNotify_Trigger` entries with a valid `NotifyTriggerName`; `None` notifies are skipped.
- `Config/DefaultEngine.ini` contains redirects for older class/property names; preserve them unless a migration is intentionally complete.
