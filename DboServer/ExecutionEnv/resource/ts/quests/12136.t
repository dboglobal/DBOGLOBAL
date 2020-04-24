CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 12136;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 254;
			prelnk = "10;";

			CDboTSActBroadMsg
			{
				dt = 5;
				et = 2;
				oidx = 7511101;
				oqtidx = 1101142;
				ot = 0;
				uisdt = 2;
				oc = 0;
				taid = 1;
				uisht = 1;
				uibst = 1;
				uidt = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;1;2;";

			CDboTSActSendSvrEvt
			{
				id = 1021;
				tblidx = -1;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSRcvSvrEvt
			{
				id = 1022;
			}
		}
		CDboTSContGCond
		{
			cid = 11;
			prelnk = "9;";
			nolnk = 255;
			rm = 0;
			yeslnk = 10;

			CDboTSRcvSvrEvt
			{
				id = 1024;
			}
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 12;
			prelnk = "11;";

			CDboTSActETimerE
			{
				taeid = 2;
				taid = 1;
				tceid = 9;
			}
			CDboTSActSendSvrEvt
			{
				id = 1026;
				tblidx = 12138;
				stype = 2;
				taid = 2;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 253;
			nextlnk = 11;
			prelnk = "7;";

			CDboTSActETimerS
			{
				sort = 0;
				taid = 2;
				tgid = 1;
				time = 1200000;
			}
			CDboTSActWorldPlayScript
			{
				sid = 12138;
				start = 1;
				taid = 1;
				uof = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 1101021;
				gtype = 2;
				area = 1100972;
				goal = 1101020;
				grade = -1;
				rwd = 255;
				sort = -1;
				stype = 4;
				taid = 1;
				title = 1101019;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "12;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 9;
			prelnk = "6;8;5;8;";

			CDboTSActSendSvrEvt
			{
				id = 1023;
				tblidx = -1;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContUsrSel
		{
			cancellnk = 7;
			cid = 6;
			lilnk = 255;
			ndesc0 = 1101017;
			uspt = 3;
			desc = -1;
			nid0 = 8;
			type = -1;
			ust = 1;
			idx = -1;
			prelnk = "5;";
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 8;
			dt = 2;
			lilnk = 255;
			oklnk = 7;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "6;";
			dg = 1101018;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 5;
			dt = 2;
			lilnk = 255;
			oklnk = 6;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "4;";
			dg = 1101016;
			gt = 0;
			oi = 7511101;
		}
		CDboTSContNarration
		{
			cancellnk = 3;
			cid = 2;
			dt = 2;
			lilnk = 255;
			oklnk = 3;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "1;";
			dg = 1101013;
			gt = 0;
			oi = 2413109;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "9;0;";
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
				id = 1020;
			}
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
			dg = 1101012;
			gt = 0;
			oi = 7511101;
		}
	}
	CNtlTSGroup
	{
		gid = 1;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActTSState
			{
				state = 2;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 2;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

