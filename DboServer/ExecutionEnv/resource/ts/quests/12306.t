CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 12306;
	title = 1230602;

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
			prelnk = "2;2;1;";

			CDboTSActSendSvrEvt
			{
				id = 23130;
				tblidx = -1;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 13;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 13;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1401142;
			gt = 0;
			oi = 2413109;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContNarration
		{
			cancellnk = 13;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1401141;
			gt = 0;
			oi = 2413109;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1230607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 23120;
			}
		}
	}
}

