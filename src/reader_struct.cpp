#include <iostream>
#include <iomanip>
#include "ldb_reader.h"
#include "lmt_reader.h"
#include "lmu_reader.h"
#include "lsd_reader.h"
#include "reader_struct.h"

template <class S>
void Struct<S>::MakeFieldMap() {
	for (int i = 0; fields[i] != NULL; i++)
		field_map[fields[i]->id] = fields[i];
}

template <class S>
void Struct<S>::ReadLcf(S& obj, Reader& stream) {
	if (field_map.empty())
		MakeFieldMap();

	Reader::Chunk chunk_info;

	ID_reader->ReadID(obj, stream);

	while (!stream.Eof()) {
		chunk_info.ID = stream.ReadInt();
		if (chunk_info.ID == 0)
			break;

		chunk_info.length = stream.ReadInt();
		if (chunk_info.length == 0)
			continue;

		typename field_map_type::const_iterator it = field_map.find(chunk_info.ID);
		if (it != field_map.end())
			it->second->ReadLcf(obj, stream, chunk_info.length);
		else
			stream.Skip(chunk_info);
	}
}

template <class S>
void Struct<S>::WriteLcf(const S& obj, Writer& stream) {
	ID_reader->WriteID(obj, stream);
	S ref = S();
	for (int i = 0; fields[i] != NULL; i++) {
		const Field<S>* field = fields[i];
		if (field->IsDefault(obj, ref))
			continue;
		stream.WriteInt(field->id);
		stream.WriteInt(field->LcfSize(obj, stream));
		field->WriteLcf(obj, stream);
	}
	stream.WriteInt(0);
}

template <class S>
int Struct<S>::LcfSize(const S& obj, Writer& stream) {
	int result = ID_reader->IDSize(obj);
	S ref = S();
	for (int i = 0; fields[i] != NULL; i++) {
		const Field<S>* field = fields[i];
		if (field->IsDefault(obj, ref))
			continue;
		result += Reader::IntSize(field->id);
		int size = field->LcfSize(obj, stream);
		result += Reader::IntSize(size);
		result += size;
	}
	result += Reader::IntSize(0);
	return result;
}

template <class S>
void Struct<S>::ReadLcf(std::vector<S>& vec, Reader& stream) {
	int count = stream.ReadInt();
	vec.resize(count);
	for (int i = 0; i < count; i++)
		TypeReader<S>::ReadLcf(vec[i], stream, 0);
}

template <class S>
void Struct<S>::WriteLcf(const std::vector<S>& vec, Writer& stream) {
	int count = vec.size();
	stream.WriteInt(count);
	for (int i = 0; i < count; i++)
		TypeReader<S>::WriteLcf(vec[i], stream);
}

template <class S>
int Struct<S>::LcfSize(const std::vector<S>& vec, Writer& stream) {
	int result = 0;
	int count = vec.size();
	result += Reader::IntSize(count);
	for (int i = 0; i < count; i++)
		result += TypeReader<S>::LcfSize(vec[i], stream);
	return result;
}

template class Struct<RPG::Actor>;
template class Struct<RPG::Animation>;
template class Struct<RPG::AnimationCellData>;
template class Struct<RPG::AnimationFrame>;
template class Struct<RPG::AnimationTiming>;
template class Struct<RPG::Attribute>;
template class Struct<RPG::BattleCommand>;
template class Struct<RPG::BattleCommands>;
template class Struct<RPG::BattlerAnimation>;
template class Struct<RPG::BattlerAnimationData>;
template class Struct<RPG::BattlerAnimationExtension>;
template class Struct<RPG::Chipset>;
template class Struct<RPG::Class>;
template class Struct<RPG::CommonEvent>;
template class Struct<RPG::Database>;
template class Struct<RPG::Encounter>;
template class Struct<RPG::Enemy>;
template class Struct<RPG::EnemyAction>;
template class Struct<RPG::Event>;
template class Struct<RPG::EventPage>;
template class Struct<RPG::EventPageCondition>;
template class Struct<RPG::Item>;
template class Struct<RPG::ItemAnimation>;
template class Struct<RPG::Learning>;
template class Struct<RPG::Map>;
template class Struct<RPG::MapInfo>;
template class Struct<RPG::MoveRoute>;
template class Struct<RPG::Music>;
template class Struct<RPG::Save>;
template class Struct<RPG::SaveActor>;
template class Struct<RPG::SaveCommonEvent>;
template class Struct<RPG::SaveEventCommands>;
template class Struct<RPG::SaveEventData>;
template class Struct<RPG::SaveEvents>;
template class Struct<RPG::SaveInventory>;
template class Struct<RPG::SaveMapEvent>;
template class Struct<RPG::SaveMapInfo>;
template class Struct<RPG::SavePartyLocation>;
template class Struct<RPG::SavePicture>;
template class Struct<RPG::SaveScreen>;
template class Struct<RPG::SaveSystem>;
template class Struct<RPG::SaveTarget>;
template class Struct<RPG::SaveTitle>;
template class Struct<RPG::SaveVehicleLocation>;
template class Struct<RPG::Skill>;
template class Struct<RPG::Sound>;
template class Struct<RPG::State>;
template class Struct<RPG::Switch>;
template class Struct<RPG::System>;
template class Struct<RPG::Terms>;
template class Struct<RPG::Terrain>;
template class Struct<RPG::TestBattler>;
template class Struct<RPG::TreeMap>;
template class Struct<RPG::Troop>;
template class Struct<RPG::TroopMember>;
template class Struct<RPG::TroopPage>;
template class Struct<RPG::TroopPageCondition>;
template class Struct<RPG::Variable>;