CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 170;
	title = 17002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 12;
			prelnk = "3;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 100;
			prelnk = "13;";

			CDboTSActMiniNarration
			{
				taid = 1;
				tidx = 1703;
			}
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 101;
			prelnk = "100;";

			CDboTSActExcCGroup
			{
				gid = 255;
				rtype = 0;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 3;
			prelnk = "2;";

			CDboTSActExcCGroup
			{
				gid = 21;
				rtype = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContUnifiedNarration
		{
			cancellnk = 1;
			cid = 4;
			idx = 1701;
			lilnk = 255;
			oklnk = 1;
			prelnk = "0;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 17007;
			nolnk = 253;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckPCCls
			{
				clsflg = 3;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 20;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "8031101;";
			}
			CDboTSCheckClrQst
			{
				and = "169;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 10;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 17009;
				ctype = 1;
				idx = 8031101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 425;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 17008;
				m1fx = "4236.000000";
				m1fy = "0.000000";
				sort = 17005;
				m0fy = "0.000000";
				m0pat = 1;
				m1fz = "4733.000000";
				grade = 132203;
				m0fx = "70.610001";
				m0ttip = 17015;
				m1pat = 0;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 17002;
				gtype = 3;
				area = 17001;
				goal = 17004;
				m0fz = "-744.640015";
				m0widx = 2;
				m1ttip = 17016;
				scitem = -1;
				stype = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 17008;
			gtype = 3;
			oklnk = 2;
			area = 17001;
			goal = 17004;
			sort = 17005;
			prelnk = "4;4;";
			ds = 0;
			grade = 132203;
			rwd = 100;
			title = 17002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 17014;
			nextlnk = 11;
			rwdexp = 0;
			rwdtbl = 17001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "12;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "8031101;";
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "11;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
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
			cid = 3;
			prelnk = "10;";
			nolnk = 255;
			rm = 0;
			yeslnk = 13;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;1;";
			type = 0;
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 254;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 425;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 425;
				taid = 1;
				type = 1;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 21;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActRegCTiming
			{
				0_midx = 6614101;
				rtype = 1;
				etype = 0;
				flags = 1;
				ntidx = 1702;
				taid = 1;
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

