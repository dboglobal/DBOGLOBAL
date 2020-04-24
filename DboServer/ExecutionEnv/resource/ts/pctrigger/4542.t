CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 4542;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;2;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 253;
			prelnk = "1;";

			CDboTSActPortal
			{
				dx = "-0.450000";
				px = "-807.250000";
				widx = 15;
				dy = "0.000000";
				py = "88.400002";
				taid = 1;
				type = 0;
				dz = "-0.890000";
				pz = "798.210022";
			}
		}
		CDboTSContUsrSel
		{
			cancellnk = 253;
			cid = 1;
			lilnk = 255;
			ndesc0 = 46;
			uspt = -1;
			desc = -1;
			nid0 = 2;
			type = -1;
			ust = 0;
			idx = -1;
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
				npcidx = 5781101;
			}
		}
	}
}

