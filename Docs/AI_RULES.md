# AI Rules

## Project Context
- Unreal Engine 5.4 C++ project: `SkillMaker`.
- Main module: `Source/SkillMaker`.
- Runtime module only. Do not introduce new plugins or modules unless explicitly requested.
- Primary gameplay loop: skill creation/editing, skill save/load, skill training, projectile/combat execution.
- Default project map is currently `/Game/SkillMaker/Map/SkillTrainingMap`.

## Editing Rules
- Preserve existing user changes. Check `git status --short` before risky edits.
- Do not modify `Content/**/*.uasset`, `*.umap`, `Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`, `.idea/`, or generated IDE files unless explicitly requested.
- Prefer C++ changes in `Source/SkillMaker` and config changes in `Config`.
- For UMG-bound widgets, keep `UPROPERTY(meta = (BindWidget))` names synchronized with widget blueprint names.
- Use the existing `SK` prefix naming convention.
- Use `TObjectPtr<>` for UObject references where the project already does.
- Use `SK_LOG(LogSkillMaker, ...)` for SkillMaker logs unless touching code that already consistently uses another category.
- Avoid broad refactors while feature or bug work is in progress.

## Unreal/C++ Rules
- After adding or changing reflected types, include the matching `.generated.h` last in the header include list.
- Keep `UCLASS`, `USTRUCT`, `UENUM`, `UFUNCTION`, and `UPROPERTY` metadata Blueprint-friendly when the type is used by UI or assets.
- DataTable row structs should derive from `FTableRowBase`.
- Networked gameplay should preserve the existing authority flow:
  - client entry: `ClientRequestUseSkill`
  - server validation/execution: `ServerUseSkill`
  - replication fanout: `MulticastExecuteSkill`
- Do not hardcode asset paths unless matching an existing project pattern, such as GameMode default class assignment.
- Prefer Blueprint-assigned references or subsystem-managed references for assets used by UI/data flows.

## Save/Skill Rules
- Treat `USKPlayerSkillSave`, `USKSaveGameSubsystem`, `USKSkillManager`, and skill editor/training widgets as separate responsibilities.
- SaveGame persistence should go through Unreal save APIs (`UGameplayStatics::SaveGameToSlot`, `LoadGameFromSlot`, `DoesSaveGameExist`).
- DataTable persistence/editing should stay separate from runtime player save data unless the task explicitly asks to merge them.
- Be careful with `FName` keys for skill IDs and skill set names. Changing key semantics can orphan existing saved data or DataTable rows.

## Validation
- At minimum, run an editor build after C++ changes.
- For UI changes, verify the corresponding widget blueprint bindings in the editor.
- For skill/combat/save changes, test in `Content/SkillMaker/Map/SkillTrainingMap.umap`.
- For DataTable-related changes, verify `DT_SkillData`, `DT_WeaponData`, `DT_AnimationData`, and `DT_ProjectileData` references in the editor.
