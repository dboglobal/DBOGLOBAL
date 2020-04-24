CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 12200;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 254;
			prelnk = "6;";

			CDboTSActBroadMsg
			{
				dt = 5;
				et = 2;
				oidx = 7511101;
				oqtidx = 1101143;
				ot = 0;
				uisdt = 2;
				oc = 0;
				taid = 1;
				uisht = 1;
				uibst = 0;
				uidt = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "7;";
			type = 1;
		}
		CDboTSContNarration
		{
			cancellnk = 13;
			cid = 3;
			dt = 2;
			lilnk = 255;
			oklnk = 13;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "2;";
			dg = 1101062;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 13;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1101061;
			gt = 0;
			oi = 2413109;
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 6;
			prelnk = "3;2;1;3;";

			CDboTSActSendSvrEvt
			{
				id = 22010;
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
			cid = 6;
			prelnk = "13;";
			nolnk = 255;
			rm = 0;
			yeslnk = 7;

			CDboTSRcvSvrEvt
			{
				id = 22380;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 22000;
			}
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
			dg = 1101060;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
	}
}

