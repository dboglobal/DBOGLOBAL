CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	rq = 0;
	tid = 4;
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

			CDboTSActPortal
			{
				dx = "1.000000";
				px = "7448.000000";
				widx = 1;
				dy = "0.000000";
				py = "-62.000000";
				taid = 1;
				type = 1;
				dz = "0.000000";
				pz = "1562.000000";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;2;";
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
				npcidx = 2972106;
			}
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
			idx = 2972106;
			prelnk = "0;";
		}
	}
}

