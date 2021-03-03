ServerToServer::ServerToServer()
{

}

ServerToServer::~ServerToServer()
{

}

bool ServerToServer::IsWorldEnabled(uint16 world) const
{
	DisabledWorldList::const_iterator it = this->disabled_world_list.find(world);

	return it == this->disabled_world_list.end() ? true: false;
}