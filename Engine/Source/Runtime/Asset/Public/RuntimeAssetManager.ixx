module;
#include <unordered_map>
#include <map>
#include <string>
export module Asset.RuntimeAssetManager;
import Core.File;
import Asset.Asset;

namespace SIByL::Asset
{
	export enum struct ResourceKind :uint64_t
	{
		TEXTURE,
		MESH,
	};

	export using TimeStamp = uint64_t;

	export struct ResourceItem
	{
		ResourceKind kind;
		TimeStamp cachedTime;
		uint64_t cacheID;
		std::string path;
	};

	export class RuntimeAssetManager
	{
	public:
		RuntimeAssetManager();
		// Manager Life Cycle
		auto initialize() noexcept -> void;
		auto flush() noexcept -> void;
		auto exit() noexcept -> void;

		// Manager Functions
		auto addNewAsset(ResourceItem const& item) noexcept -> GUID;
		auto addNewAssetWithGUID(ResourceItem const& item, GUID const& guid) noexcept -> void;
		auto findAsset(GUID const& guid) noexcept -> ResourceItem*;

		auto getAssetMap() const noexcept -> std::map<GUID, ResourceItem> const& { return assetsMap; }
		auto getAssetLoader() noexcept -> Core::AssetLoader* { return &assetLoader; }

	private:
		auto serialize() noexcept -> void;
		auto deserialize() noexcept -> void;
		Core::AssetLoader assetLoader;
		std::map<GUID, ResourceItem> assetsMap;
		std::unordered_map<std::string, GUID> inverseMap;
	};
}