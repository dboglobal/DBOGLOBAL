CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 13107;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 1;
			prelnk = "0;";

			CDboTSActSendSvrEvt
			{
				id = 669;
				tblidx = 7611209;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 222;
			prelnk = "3;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 222;
			prelnk = "2;";

			CDboTSActEscort
			{
				start = 0;
				tblidx = 7611209;
				tsshare = 1;
				etype = 1;
				taid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 255;
			rm = 0;
			yeslnk = 3;

			CDboTSRcvSvrEvt
			{
				id = 639;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "4;";

			CDboTSActEscort
			{
				start = 1;
				tblidx = 7611209;
				tsshare = 1;
				etype = 1;
				taid = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSRcvSvrEvt
			{
				id = 640;
			}
		}
	}
}

