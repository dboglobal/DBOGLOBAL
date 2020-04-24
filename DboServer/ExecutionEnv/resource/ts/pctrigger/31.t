CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	rq = 0;
	tid = 31;
	title = 3102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 3107;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSColObject
			{
				objidx = "1021;";
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
				dx = "0.297000";
				px = "101.872002";
				widx = 10;
				dy = "0.000000";
				py = "71.294998";
				taid = 1;
				type = 1;
				dz = "0.955000";
				pz = "-728.841003";
			}
		}
	}
}

