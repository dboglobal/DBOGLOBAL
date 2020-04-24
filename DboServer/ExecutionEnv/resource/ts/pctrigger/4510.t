CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	rq = 0;
	tid = 4510;
	title = 451002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActPIDgn
			{
				hidx = -1;
				nidx = -1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 451007;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSColObject
			{
				objidx = "29;";
				widx = 710000;
			}
		}
	}
}

