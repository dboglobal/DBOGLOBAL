CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 13103;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 3;
			prelnk = "2;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;1;";

			CDboTSActSendSvrEvt
			{
				id = 650;
				tblidx = 70110;
				stradius = 0;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 649;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 2;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1101256;
			gt = 0;
			oi = 7611209;
		}
	}
}

