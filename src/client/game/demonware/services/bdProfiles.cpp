#include <std_include.hpp>
#include "../services.hpp"
#include "bdStorage.hpp"

#include "../../../component/profile_infos.hpp"

namespace demonware
{
	bdProfiles::bdProfiles() : service(8, "bdProfiles")
	{
		this->register_task(1, &bdProfiles::getPublicInfos);
		this->register_task(2, &bdProfiles::getPrivateInfo);
		this->register_task(3, &bdProfiles::setPublicInfo);
		this->register_task(4, &bdProfiles::setPrivateInfo);
		this->register_task(5, &bdProfiles::deleteProfile);
		this->register_task(6, &bdProfiles::setPrivateInfoByUserID);
		this->register_task(7, &bdProfiles::getPrivateInfoByUserID);
		this->register_task(8, &bdProfiles::setPublicInfoByUserID);
	}

	void bdProfiles::getPublicInfos(service_server* server, byte_buffer* buffer) const
	{
		const auto server_lobby = game::SV_MainMP_GetServerLobby();
		const auto* svs_clients = *game::svs_clients;
		for (unsigned int i = 0; i < *game::svs_numclients; ++i)
		{
			if (svs_clients[i].header.state >= 1)
			{
				game::XUID xuid{};
				xuid.m_id = game::Session_GetXuid(server_lobby, i);
				printf("PlayercardCache_AddToDownload(%llX)\n", xuid.m_id);
				game::PlayercardCache_AddToDownload(0, xuid);
			}
		}

		std::vector<std::pair<std::uint64_t, profile_infos::profile_info>> profile_infos{};

		std::uint64_t entity_id;
		while (buffer->read_uint64(&entity_id))
		{
			auto profile = profile_infos::get_profile_info(entity_id);
			if (profile)
			{
				profile_infos.emplace_back(entity_id, std::move(*profile));
			}
		}

		auto reply = server->create_reply(this->task_id(), profile_infos.empty() ? game::BD_NO_PROFILE_INFO_EXISTS : game::BD_NO_ERROR);

		for (auto& info : profile_infos)
		{
			auto result = new bdPublicProfileInfo;
			result->m_entityID = info.first;
			result->m_memberplayer_card = std::move(info.second.m_memberplayer_card);

			reply->add(result);
		}

		reply->send();
	}

	void bdProfiles::setPublicInfo(service_server* server, byte_buffer* buffer) const
	{
		profile_infos::profile_info info{};
		buffer->read_blob(&info.m_memberplayer_card);
		profile_infos::update_profile_info(info);

		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdProfiles::getPrivateInfo(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdProfiles::setPrivateInfo(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdProfiles::deleteProfile(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdProfiles::setPrivateInfoByUserID(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdProfiles::getPrivateInfoByUserID(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}

	void bdProfiles::setPublicInfoByUserID(service_server* server, byte_buffer* /*buffer*/) const
	{
		// TODO:
		auto reply = server->create_reply(this->task_id());
		reply->send();
	}
}
