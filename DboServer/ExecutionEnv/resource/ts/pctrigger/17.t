CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	rq = 0;
	tid = 17;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = 2971106;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;1;";
			type = 0;
		}
		CDboTSContUsrSel
		{
			cancellnk = 254;
			cid = 1;
			lilnk = 255;
			ndesc0 = 5;
			uspt = 0;
			desc = 1;
			nid0 = 2;
			type = 0;
			ust = 0;
			idx = 2971106;
			prelnk = "0;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActPortal
			{
				dx = "1.000000";
				px = "7448.507813";
				widx = 1;
				dy = "0.000000";
				py = "-79.721001";
				taid = 1;
				type = 1;
				dz = "0.000000";
				pz = "1561.874023";
			}
		}
	}
}

