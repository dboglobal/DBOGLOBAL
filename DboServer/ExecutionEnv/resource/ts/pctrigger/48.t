CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	rq = 0;
	tid = 48;
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
				dx = "-0.777000";
				px = "-3983.372070";
				widx = 1;
				dy = "0.000000";
				py = "-142.748001";
				taid = 1;
				type = 1;
				dz = "0.630000";
				pz = "-4085.997070";
			}
		}
		CDboTSContUsrSel
		{
			cancellnk = 254;
			cid = 1;
			lilnk = 255;
			ndesc0 = 37;
			uspt = -1;
			desc = 39;
			nid0 = 2;
			type = 0;
			ust = 0;
			idx = 6112103;
			prelnk = "0;";
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
				npcidx = 6112103;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;1;";
			type = 0;
		}
	}
}

