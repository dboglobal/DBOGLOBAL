CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 12209;
	title = 1101057;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1101058;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = 7511101;
			}
			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 12210;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActSendSvrEvt
			{
				id = 22280;
				tblidx = -1;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
	}
}

