CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 12308;
	title = 1230802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "13;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;1;";

			CDboTSActSendSvrEvt
			{
				id = 23170;
				tblidx = -1;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1230807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 23160;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 13;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 13;
			ot = 0;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1401135;
			gt = 0;
			oi = 7511101;
		}
	}
}

