CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	rq = 0;
	tid = 1;
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
				objidx = "558;";
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
				dx = "-49.000000";
				px = "914.000000";
				widx = 2;
				dy = "-0.780000";
				py = "-106.000000";
				taid = 1;
				type = 1;
				dz = "0.390000";
				pz = "-1104.000000";
			}
		}
	}
}

