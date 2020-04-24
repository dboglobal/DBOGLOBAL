CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 3757;
	title = 375702;

	CNtlTSGroup
	{
		gid = 0;

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
			cid = 13;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 375714;
			nextlnk = 11;
			rwdexp = 0;
			rwdtbl = 375701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "13;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3451101;";
			}
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
			stdiag = 375707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "3451101;";
			}
			CDboTSCheckClrQst
			{
				and = "3753;";
				flink = 1;
				flinknextqid = "3762;";
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
				conv = 375709;
				ctype = 1;
				idx = 3451101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 12953300;
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
				cont = 375708;
				m1fx = "4236.000000";
				m1fy = "0.000000";
				sort = 375705;
				m0fy = "0.000000";
				m0pat = 1;
				m1fz = "4733.000000";
				grade = 132203;
				m0fx = "-3866.149902";
				m0ttip = 375715;
				m1pat = 0;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 375702;
				gtype = 3;
				area = 375701;
				goal = 375704;
				m0fz = "-5705.700195";
				m0widx = 15;
				m1ttip = 375716;
				scitem = -1;
				stype = 2;
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
				itype = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 375708;
			gtype = 3;
			oklnk = 2;
			area = 375701;
			goal = 375704;
			sort = 375705;
			prelnk = "0;";
			ds = 0;
			grade = 132203;
			rwd = 100;
			title = 375702;
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
				0_midx = 12953300;
				rtype = 1;
				etype = 0;
				flags = 1;
				ntidx = 3757;
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
			stdiag = 375707;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

