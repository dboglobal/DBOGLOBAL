CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3015;
	title = 301502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 301507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 58;
			}
			CDboTSClickNPC
			{
				npcidx = "4501109;";
			}
			CDboTSCheckClrQst
			{
				and = "3014;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 5;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 301509;
				ctype = 1;
				idx = 4501109;
				taid = 2;
			}
			CDboTSActItem
			{
				iidx0 = 99151;
				stype0 = 1;
				taid = 4;
				type = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 301508;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 301505;
				m2widx = 1;
				grade = -1;
				m1pat = 2;
				m1widx = 1;
				m3fx = "4371.649902";
				rwd = 100;
				taid = 1;
				gtype = 1;
				m3fy = "0.000000";
				area = 301501;
				goal = 301504;
				m0fz = "-4932.209961";
				m0widx = 1;
				m1ttip = 301515;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-5032.919922";
				m3ttip = 301515;
				scitem = 99151;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-5056.810059";
				m2fx = "4451.399902";
				m2ttip = 301515;
				m0fx = "4538.250000";
				m0ttip = 301515;
				m3pat = 2;
				title = 301502;
				m1fx = "4654.060059";
				m2fz = "-5012.149902";
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 3015;
				cnt0 = 6;
				ectype = -1;
				etype = 0;
				qtidx = 301515;
				taid = 3;
				esctype = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 301514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 301501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4501109;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
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
				eitype = -1;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
			CDboTSActItem
			{
				iidx0 = 99151;
				stype0 = 1;
				taid = 2;
				type = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 301508;
			gtype = 1;
			oklnk = 2;
			area = 301501;
			goal = 301504;
			sort = 301505;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 301502;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 99151;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 301507;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
		}
	}
}

