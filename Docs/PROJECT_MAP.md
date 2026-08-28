# Project Map

## Root
- `SkillMaker.uproject`: UE 5.4 project, runtime module `SkillMaker`.
- `Source/SkillMaker.Target.cs`: game target.
- `Source/SkillMakerEditor.Target.cs`: editor target.
- `Config/DefaultEngine.ini`: default map, GameMode, redirects, renderer/platform settings.
- `Config/DefaultInput.ini`: Enhanced Input defaults.
- `Content/SkillMaker`: project-owned gameplay assets, maps, data tables, input assets, UI widgets.

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
  - `FSKSkillData`: skill row data, montage, cooldown, damage, projectile, status effects.
  - `USKSkillComponent`: skill map, cooldowns, server/multicast skill execution.
  - `ASKProjectileActor`: projectile behavior.
  - `USKSkillManager`: skill data manager object.
- `Source/SkillMaker/Combat/`
  - `USKCombatComponent`: replicated health, damage, status effects, death.
- `Source/SkillMaker/Data/`
  - `FSKWeaponData`, `FSKAnimationData`, `FSKProjectileData`: DataTable row structs.
  - `USKDataManager`: older UObject-style data manager.
- `Source/SkillMaker/Game/`
  - `USKDataManagerSubsystem`: GameInstanceSubsystem exposing weapon/animation/projectile table lists.
  - `USKSaveGameSubsystem`: save/load access for player skill sets.
  - `USKGameInstance`: project GameInstance class.
- `Source/SkillMaker/Save/`
  - `USKPlayerSkillSave`: SaveGame object containing current and named player skill sets.
- `Source/SkillMaker/GameMode/`
  - `ASKSkillMakerGameMode`: editor/skill maker flow, editor HUD.
  - `ASKSkillTrainGameMode`: training flow, player pawn/controller, training HUD.
- `Source/SkillMaker/UI/UI-SkillMaker/`
  - `UEEditor/`: skill maker editor HUD/main widget.
  - `TrainingRoom/`: training room HUD/main widget.
  - `Common/`: reusable selection/detail widgets for skills, weapons, animations, projectiles, status effects.
- `Source/SkillMaker/Animation/`
  - custom skill animation notifies and player anim instance.
- `Source/SkillMaker/Prop/`
  - interactable actors and skill station.
- `Source/SkillMaker/Logging/`
  - `LogSkillMaker` category and `SK_LOG` helper macro.

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
  - editor: `WBP_SKSkillMakerEditorMain`, `WBP_SKSkillMakeEditorHUD`
  - training: `WBP_SKSkillMakerTrainMain`, `WBP_SKSkillMakerTrainHUD`
  - shared cards/selection widgets under `Content/SkillMaker/UI`.

## Current Notes
- `DefaultEngine.ini` sets `GlobalDefaultGameMode` to `/Script/SkillMaker.SkillMakerGameMode`.
- `ASKSkillTrainGameMode` assigns `BP_SKPlayerCharacter`, `BP_SKPlayerController`, and `ASKSkillMakerTrainHUD`.
- `ASKSkillMakerGameMode` assigns `BP_SKSkillMakerController` and `ASKSkillMakerEditorHUD`.
- DataTable references in `USKDataManagerSubsystem` are code-side properties. Verify assignment location in Blueprint/assets before changing data loading.
- `Config/DefaultEngine.ini` contains redirects for older class/property names; preserve them unless a migration is intentionally complete.
