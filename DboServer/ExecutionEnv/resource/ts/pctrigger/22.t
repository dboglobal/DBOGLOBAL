CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	rq = 0;
	tid = 22;
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

			CDboTSColObject
			{
				objidx = "565;";
				widx = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActPortal
			{
				dx = "-0.420000";
				px = "355.000000";
				widx = 9;
				dy = "-0.340000";
				py = "20.000000";
				taid = 1;
				type = 1;
				dz = "0.840000";
				pz = "-393.000000";
			}
		}
	}
}

