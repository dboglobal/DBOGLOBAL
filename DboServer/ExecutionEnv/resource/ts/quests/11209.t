CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11209;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 242;
			prelnk = "21;";
			nolnk = 243;
			rm = 0;
			yeslnk = 22;

			CDboTSCheckInWorld
			{
				widx = 192000;
			}
		}
		CDboTSContGAct
		{
			cid = 243;
			elnk = 255;
			nextlnk = 14;
			prelnk = "242;";

			CDboTSActRegQInfo
			{
				cont = 1100200;
				gtype = 2;
				area = 1100195;
				goal = 1100199;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100198;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 1;
				eitype = 0;
				idx0 = 7112412;
				cnt0 = 1;
				cnt2 = 3;
				ectype = -1;
				etype = 0;
				idx1 = 7111402;
				taid = 2;
				esctype = 0;
				idx2 = 9615100;
			}
		}
		CDboTSContGAct
		{
			cid = 18;
			elnk = 255;
			nextlnk = 19;
			prelnk = "17;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 27;
			elnk = 255;
			nextlnk = 3;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = -1;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 16;
			prelnk = "24;";
			nolnk = 255;
			rm = 0;
			yeslnk = 17;

			CDboTSRcvSvrEvt
			{
				id = 12311;
			}
		}
		CDboTSContGAct
		{
			cid = 17;
			elnk = 255;
			nextlnk = 18;
			prelnk = "16;";

			CDboTSActNPCConv
			{
				conv = 1100471;
				ctype = 1;
				idx = 5551103;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 21;
			elnk = 255;
			nextlnk = 242;
			prelnk = "20;";

			CDboTSActSToCEvt
			{
				apptype = 0;
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
		}
		CDboTSContGAct
		{
			cid = 24;
			elnk = 255;
			nextlnk = 16;
			prelnk = "23;";

			CDboTSActSToCEvt
			{
				apptype = 0;
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
		}
		CDboTSContGCond
		{
			cid = 23;
			prelnk = "15;";
			nolnk = 255;
			rm = 0;
			yeslnk = 24;

			CDboTSRcvSvrEvt
			{
				id = 12293;
			}
		}
		CDboTSContGAct
		{
			cid = 22;
			elnk = 255;
			nextlnk = 14;
			prelnk = "242;";

			CDboTSActRegQInfo
			{
				cont = 1100200;
				gtype = 2;
				area = 1100195;
				goal = 1100199;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100198;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 1;
				eitype = 0;
				idx0 = 7112416;
				cnt0 = 1;
				cnt2 = 3;
				ectype = -1;
				etype = 0;
				idx1 = 7111402;
				taid = 2;
				esctype = 0;
				idx2 = 9615100;
			}
		}
		CDboTSContGCond
		{
			cid = 20;
			prelnk = "13;";
			nolnk = 255;
			rm = 0;
			yeslnk = 21;

			CDboTSRcvSvrEvt
			{
				id = 12273;
			}
		}
		CDboTSContGAct
		{
			cid = 241;
			elnk = 255;
			nextlnk = 2;
			prelnk = "240;";

			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 2;
				eitype = 0;
				idx0 = 7114302;
				cnt0 = 1;
				cnt2 = 2;
				ectype = -1;
				etype = 0;
				idx1 = 7114300;
				taid = 2;
				esctype = 0;
				idx2 = 7112411;
			}
			CDboTSActRegQInfo
			{
				cont = 1100197;
				gtype = 1;
				area = 1100195;
				goal = 1100196;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100194;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "19;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 12;
			prelnk = "3;3;";

			CDboTSActSendSvrEvt
			{
				id = 12260;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 2;
				ttype = 255;
				type = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 1100200;
				gtype = 2;
				area = 1100195;
				goal = 1100199;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100198;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 4;
				eitype = 0;
				idx0 = 7111402;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 5214100;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "241;1;";
			nolnk = 255;
			rm = 0;
			yeslnk = 27;

			CDboTSRcvSvrEvt
			{
				id = 12250;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 4;
			cid = 3;
			dt = 0;
			lilnk = 255;
			oklnk = 4;
			ot = 0;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "27;";
			dg = 1100167;
			gt = 0;
			oi = 5551103;
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
			cid = 15;
			elnk = 255;
			nextlnk = 23;
			prelnk = "14;";

			CDboTSActNPCConv
			{
				conv = 1100470;
				ctype = 1;
				idx = 5551103;
				taid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 14;
			prelnk = "243;22;";
			nolnk = 255;
			rm = 0;
			yeslnk = 15;

			CDboTSRcvSvrEvt
			{
				id = 12292;
			}
		}
		CDboTSContGAct
		{
			cid = 19;
			elnk = 255;
			nextlnk = 254;
			prelnk = "18;";

			CDboTSActNPCConv
			{
				conv = 1100472;
				ctype = 1;
				idx = 2761102;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 20;
			prelnk = "12;";

			CDboTSActNPCConv
			{
				conv = 1100469;
				ctype = 1;
				idx = 5551103;
				taid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 12;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 13;

			CDboTSRcvSvrEvt
			{
				id = 12272;
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
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 240;

			CDboTSRcvSvrEvt
			{
				id = 12240;
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
				cont = 1100197;
				gtype = 1;
				area = 1100195;
				goal = 1100196;
				grade = -1;
				rwd = 255;
				sort = 1100024;
				stype = 4;
				taid = 1;
				title = 1100194;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cnt1 = 2;
				eitype = 0;
				idx0 = 7114302;
				cnt0 = 1;
				cnt2 = 2;
				ectype = -1;
				etype = 0;
				idx1 = 7114300;
				taid = 2;
				esctype = 0;
				idx2 = 7112415;
			}
		}
	}
}

