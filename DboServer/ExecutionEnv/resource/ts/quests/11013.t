CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 11013;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 253;
			elnk = 255;
			nextlnk = 254;
			prelnk = "252;";

			CDboTSActSendSvrEvt
			{
				id = 1101303;
				tblidx = -1;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 252;
			prelnk = "1;";
			nolnk = 255;
			rm = 0;
			yeslnk = 253;

			CDboTSRcvSvrEvt
			{
				id = 1101302;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 252;
			prelnk = "0;";

			CDboTSActSendSvrEvt
			{
				id = 1101301;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "253;0;";
			type = 1;
		}
	}
}

