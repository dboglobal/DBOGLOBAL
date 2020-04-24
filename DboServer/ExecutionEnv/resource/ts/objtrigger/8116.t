CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 8116;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActSendSvrEvt
			{
				id = 485;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;

			CDboTSClickObject
			{
				objidx = "50;";
				widx = 715000;
			}
		}
	}
}

