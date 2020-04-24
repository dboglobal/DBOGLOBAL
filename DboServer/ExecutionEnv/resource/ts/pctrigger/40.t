CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	rq = 0;
	tid = 40;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActTLQ
			{
				idx = -1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = 4151110;
			}
		}
		CDboTSContUsrSel
		{
			cancellnk = 254;
			cid = 1;
			lilnk = 255;
			ndesc0 = 97;
			uspt = -1;
			desc = 96;
			nid0 = 2;
			type = 0;
			ust = 0;
			idx = 4151110;
			prelnk = "0;";
		}
	}
}

