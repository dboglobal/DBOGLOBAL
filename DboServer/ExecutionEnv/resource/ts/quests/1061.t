CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1061;
	title = 106102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 3;
				idx0 = 313;
				cnt0 = 1;
				cnt2 = 1;
				ectype = -1;
				etype = 0;
				idx1 = 314;
				taid = 1;
				esctype = 0;
				idx2 = 315;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 106108;
			gtype = 2;
			oklnk = 2;
			area = 106101;
			goal = 106104;
			sort = 106105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 106102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 106114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 106101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 314;
			}
			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 313;
			}
			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 315;
			}
			CDboTSClickNPC
			{
				npcidx = "8031401;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;1;0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 106107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 39;
			}
			CDboTSCheckClrQst
			{
				and = "1060;";
				flink = 1;
				flinknextqid = "1071;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "8031401;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 106108;
				m1fx = "2153.000000";
				m2fz = "1353.000000";
				m1fy = "0.000000";
				sort = 106105;
				m2widx = 1;
				grade = 132203;
				m1pat = 0;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 106101;
				goal = 106104;
				m0fz = "2190.000000";
				m0widx = 1;
				m1ttip = 106115;
				m2fy = "0.000000";
				m2pat = 0;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				m1fz = "1708.000000";
				m2fx = "3051.000000";
				m2ttip = 106116;
				m0fx = "2326.000000";
				m0ttip = 106117;
				title = 106102;
			}
			CDboTSActNPCConv
			{
				conv = 106109;
				ctype = 1;
				idx = 8031401;
				taid = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
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
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 3;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
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
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt2 = 255;
				iprob2 = "1.000000";
				icnt1 = 255;
				iidx0 = 313;
				iidx2 = 315;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
				icnt0 = 255;
				iidx1 = 314;
				iprob0 = "1.000000";
			}
		}
	}
}

