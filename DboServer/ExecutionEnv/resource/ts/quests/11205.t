CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11205;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 241;
			elnk = 255;
			nextlnk = 2;
			prelnk = "240;";

			CDboTSActRegQInfo
			{
				cont = 1100187;
				gtype = 0;
				area = 1100185;
				goal = 1100186;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 2;
				title = 1100184;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 1;
				eitype = 0;
				idx0 = 7112411;
				cnt0 = 5;
				cnt2 = 4;
				ectype = -1;
				etype = 0;
				idx1 = 7112412;
				taid = 1;
				esctype = 0;
				idx2 = 6014100;
			}
		}
		CDboTSContGCond
		{
			cid = 7;
			prelnk = "6;";
			nolnk = 255;
			rm = 0;
			yeslnk = 8;

			CDboTSRcvSvrEvt
			{
				id = 12170;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 9;
			cid = 8;
			dt = 0;
			lilnk = 255;
			oklnk = 9;
			ot = 1;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "7;";
			dg = 1100166;
			gt = 0;
			oi = 4611201;
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActSendSvrEvt
			{
				id = 12150;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 2;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;";

			CDboTSActETimerE
			{
				taeid = 3;
				taid = 3;
				tceid = 4;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActSendSvrEvt
			{
				id = 12160;
				tblidx = -1;
				stradius = 0;
				stype = 2;
				taid = 2;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 10;
			prelnk = "9;";
			nolnk = 255;
			rm = 0;
			yeslnk = 11;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 0;
				eitype = 0;
				idx0 = 4531100;
				cnt0 = 8;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 1100190;
				gtype = 2;
				area = 1100185;
				goal = 1100189;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 2;
				title = 1100188;
			}
			CDboTSActETimerS
			{
				sort = 0;
				taid = 3;
				tgid = 1;
				time = 420000;
			}
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 10;
			prelnk = "8;8;";

			CDboTSActRegQInfo
			{
				cont = 1100193;
				gtype = 3;
				area = 1100185;
				goal = 1100192;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100191;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 2;
				eitype = 0;
				idx0 = 9615100;
				cnt0 = 3;
				cnt2 = 6;
				ectype = -1;
				etype = 0;
				idx1 = 7111403;
				taid = 2;
				esctype = 0;
				idx2 = 7114302;
			}
		}
		CDboTSContGCond
		{
			cid = 240;
			prelnk = "0;";
			nolnk = 241;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckInWorld
			{
				widx = 192000;
			}
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 254;
			prelnk = "10;";

			CDboTSActSendSvrEvt
			{
				id = 12152;
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
			cid = 2;
			prelnk = "241;1;";
			nolnk = 255;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "240;";

			CDboTSActRegQInfo
			{
				cont = 1100187;
				gtype = 0;
				area = 1100185;
				goal = 1100186;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 2;
				title = 1100184;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 1;
				eitype = 0;
				idx0 = 7112415;
				cnt0 = 5;
				cnt2 = 4;
				ectype = -1;
				etype = 0;
				idx1 = 7112416;
				taid = 1;
				esctype = 0;
				idx2 = 6014100;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "11;";
			type = 1;
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
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 240;

			CDboTSRcvSvrEvt
			{
				id = 12140;
			}
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

