CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	rq = 0;
	tid = 29;
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
				dx = "-0.360000";
				px = "-5451.000000";
				widx = 1;
				dy = "0.000000";
				py = "0.000000";
				taid = 1;
				type = 1;
				dz = "0.660000";
				pz = "-5564.000000";
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
				npcidx = 7611207;
			}
			CDboTSCheckClrQst
			{
				and = "742;";
			}
		}
		CDboTSContUsrSel
		{
			cancellnk = 254;
			cid = 1;
			lilnk = 255;
			ndesc0 = 30;
			uspt = -1;
			desc = 32;
			nid0 = 2;
			type = 0;
			ust = 0;
			idx = 7611207;
			prelnk = "0;";
		}
	}
}

