CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 12206;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 9;
			prelnk = "8;";

			CDboTSActBroadMsg
			{
				dt = 5;
				et = 2;
				oidx = 7511101;
				oqtidx = 1101087;
				ot = 0;
				uisdt = 2;
				oc = 0;
				taid = 1;
				uisht = 1;
				uibst = 0;
				uidt = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 8;
			prelnk = "7;7;6;";

			CDboTSActWorldPlayScript
			{
				sid = 12206;
				start = 0;
				taid = 2;
				uof = 0;
			}
			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
			CDboTSActSendSvrEvt
			{
				id = 599;
				tblidx = 2521101;
				stradius = 0;
				stype = 2;
				taid = 3;
				ttype = 255;
				type = 0;
			}
			CDboTSActSendSvrEvt
			{
				id = 598;
				tblidx = 2761901;
				stradius = 0;
				stype = 2;
				taid = 4;
				ttype = 255;
				type = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "9;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 254;
			prelnk = "11;";

			CDboTSActSendSvrEvt
			{
				id = 22220;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 253;
			nextlnk = 4;
			prelnk = "2;1;2;";

			CDboTSActWorldPlayScript
			{
				sid = 12206;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSRcvSvrEvt
			{
				id = 22200;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 11;
			prelnk = "10;";

			CDboTSActBroadMsg
			{
				dt = 5;
				et = 2;
				oidx = 7511101;
				oqtidx = 1101086;
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
			cid = 253;
			prelnk = "3;0;";
			type = 0;
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
				id = 22190;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 10;
			cid = 6;
			dt = 2;
			lilnk = 255;
			oklnk = 7;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "4;";
			dg = 1101084;
			gt = 0;
			oi = 2761901;
		}
		CDboTSContNarration
		{
			cancellnk = 10;
			cid = 7;
			dt = 2;
			lilnk = 255;
			oklnk = 10;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "6;";
			dg = 1101085;
			gt = 0;
			oi = 2521101;
		}
		CDboTSContNarration
		{
			cancellnk = 3;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1101083;
			gt = 0;
			oi = 2521101;
		}
		CDboTSContNarration
		{
			cancellnk = 3;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1101082;
			gt = 0;
			oi = 2761901;
		}
	}
}

