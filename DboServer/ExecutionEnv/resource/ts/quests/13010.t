CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 1;
	tid = 13010;
	title = 1300002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "44;44;";

			CDboTSActSendSvrEvt
			{
				id = 682;
				tblidx = 70004;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 6;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 44;

			CDboTSRcvSvrEvt
			{
				id = 681;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 101;
			cid = 44;
			dt = 2;
			lilnk = 255;
			oklnk = 101;
			ot = 0;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1101204;
			gt = 0;
			oi = 7031101;
		}
	}
}

