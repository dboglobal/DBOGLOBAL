CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1775;
	title = 177502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 105;
			elnk = 255;
			nextlnk = 110;
			prelnk = "102;101;";

			CDboTSActItem
			{
				iidx0 = 99060;
				stype0 = 1;
				taid = 2;
				type = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = -1;
				cnt0 = 0;
				ectype = -1;
				etype = 1;
				qtidx = -1;
				taid = 1;
				esctype = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "5;";
			nolnk = 102;
			rm = 1;
			yeslnk = 101;

			CDboTSCheckPCCls
			{
				clsflg = 418217;
			}
		}
		CDboTSContGCond
		{
			cid = 110;
			prelnk = "105;";
			nolnk = 100;
			rm = 1;
			yeslnk = 254;

			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 1775;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActItem
			{
				iidx0 = 99060;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 4;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99060;
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
			prelnk = "110;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 101;
			rwdzeny = 0;
			desc = 177514;
			nextlnk = 105;
			rwdexp = 0;
			rwdtbl = 177501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2611103;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 102;
			rwdzeny = 0;
			desc = 177514;
			nextlnk = 105;
			rwdexp = 0;
			rwdtbl = 177502;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2611103;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 177509;
				ctype = 1;
				idx = 2611103;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 177501;
				cnt0 = 1;
				ectype = -1;
				etype = 0;
				qtidx = 177515;
				taid = 3;
				esctype = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 177508;
				m1fx = "329.000000";
				m1fy = "0.000000";
				sort = 177505;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "310.000000";
				grade = 132203;
				m0fx = "-0.030000";
				m0ttip = 177515;
				m1pat = 2;
				m1widx = 10;
				rwd = 100;
				taid = 1;
				title = 177502;
				gtype = 3;
				area = 177501;
				goal = 177504;
				m0fz = "-27.000000";
				m0widx = 13;
				m1ttip = 177515;
				scitem = -1;
				stype = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 177508;
			gtype = 3;
			oklnk = 2;
			area = 177501;
			goal = 177504;
			sort = 177505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 177502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 177514;
			nextlnk = 255;
			rwdexp = 0;
			rwdtbl = 177501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "110;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2611103;";
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
			stdiag = 177507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 50;
			}
			CDboTSClickNPC
			{
				npcidx = "2611103;";
			}
			CDboTSCheckClrQst
			{
				and = "1772;1773;";
				flink = 1;
				flinknextqid = "1776;";
				not = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSCheckSToCEvt
			{
				itype = 5;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 99060;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 177507;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
	}
}

