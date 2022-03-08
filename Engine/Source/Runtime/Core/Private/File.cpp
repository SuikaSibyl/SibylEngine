module;
#pragma warning(disable:4996)
#include <iostream>
#include <vector>
#include <chrono>
#include <filesystem>
module Core.File;
import Core.SObject;
import Core.Log;

namespace SIByL
{
	inline namespace Core
	{
		auto AssetLoader::addSearchPath(std::filesystem::path const& path) noexcept -> void
		{
			for (auto iter = searchPathes.begin(); iter != searchPathes.end();)
			{
				if (*iter == path)
					return;
			}
			searchPathes.emplace_back(path);
		}

		auto AssetLoader::removeSearchPath(std::filesystem::path const& path) noexcept -> void
		{
			for (auto iter = searchPathes.begin(); iter != searchPathes.end();)
			{
				if (*iter == path)
				{
					iter = searchPathes.erase(iter);
				}
				else
					iter++;
			}
		}

		auto AssetLoader::fileExists(std::filesystem::path filePath) noexcept -> bool
		{
			AssetFilePtr fp = openFile(filePath);
			if (fp != nullptr) {
				closeFile(fp);
				return true;
			}
			return false;
		}

		auto AssetLoader::openFile(std::filesystem::path const& name) noexcept -> AssetFilePtr
		{
			FILE* fp = nullptr;

			for (int i = 0; i < searchPathes.size(); i++)
			{
				std::filesystem::path fullpath = searchPathes[i] / name;
				fp = fopen(fullpath.string().c_str(), "rb");
				if (fp) return (AssetFilePtr*)fp;
			}

			return nullptr;
		}

		auto AssetLoader::openFileWB(std::filesystem::path const& name) noexcept -> AssetFilePtr
		{
			FILE* fp = nullptr;

			for (int i = 0; i < searchPathes.size(); i++)
			{
				std::filesystem::path fullpath = searchPathes[i] / name;
				fp = fopen(fullpath.string().c_str(), "wb");
				if (fp) return (AssetFilePtr*)fp;
			}

			return nullptr;
		}

		auto AssetLoader::closeFile(AssetFilePtr& fp) noexcept -> void
		{
			fclose((FILE*)fp);
			fp = nullptr;
		}

		auto AssetLoader::getSize(AssetFilePtr& fp) noexcept -> size_t
		{
			FILE* _fp = static_cast<FILE*>(fp);
			long pos = ftell(_fp);
			fseek(_fp, 0, SEEK_END);
			size_t length = ftell(_fp);
			fseek(_fp, pos, SEEK_SET);
			return length;
		}

		auto AssetLoader::syncReadAll(AssetFilePtr& fp, Buffer& buf) -> void
		{
			if (!fp) {
				SE_CORE_ERROR("CORE :: syncRead failed, null file discriptor\n");
				return;
			}

			size_t size = getSize(fp);
			buf = std::move(Buffer(size, 1));
			fread(buf.getData(), buf.getSize(), 1, static_cast<FILE*>(fp));
		}

		auto AssetLoader::readBuffer(AssetFilePtr& fp, Buffer const& buf) -> void
		{
			fread(buf.getData(), buf.getSize(), 1, static_cast<FILE*>(fp));
		}

		auto AssetLoader::syncReadAll(std::filesystem::path const& name, Buffer& buf) -> void
		{
			AssetFilePtr fp = openFile(name);
			if (fp)
			{
				syncReadAll(fp, buf);
				closeFile(fp);
			}
		}

		auto AssetLoader::syncWriteAll(std::filesystem::path const& name, Buffer& buf) -> void
		{
			AssetFilePtr fp = openFileWB(name);
			if (fp)
			{
				fwrite(buf.getData(), buf.getSize(), 1, static_cast<FILE*>(fp));
				closeFile(fp);
			}
		}

		auto AssetLoader::writeBuffer(AssetFilePtr& fp, Buffer const& buf) -> void
		{
			fwrite(buf.getData(), buf.getSize(), 1, static_cast<FILE*>(fp));
		}

		auto AssetLoader::findRelativePath(std::filesystem::path const& relative) noexcept -> std::filesystem::path
		{
			for (int i = 0; i < searchPathes.size(); i++)
			{
				std::filesystem::path fullpath = searchPathes[i] / relative;
				if (std::filesystem::exists(fullpath)) return fullpath;
			}
			return {};
		}

		auto AssetLoader::getFileLastWriteTime(std::filesystem::path const& relative) noexcept -> uint64_t
		{
			auto ftime = std::filesystem::last_write_time(findRelativePath(relative));
			auto duration = ftime.time_since_epoch();
			return duration.count();
		}
	}
}