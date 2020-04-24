CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1334;
	title = 133402;

	CNtlTSGroup
	{
		gid = 0;

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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 133407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 43;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1335;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4372211;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 133409;
				ctype = 1;
				idx = 4372211;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
				idx0 = 835;
				cnt0 = 8;
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
				cont = 133408;
				m1fx = "2948.000000";
				m1fy = "0.000000";
				sort = 133405;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "4287.000000";
				grade = 132203;
				m0fx = "2776.000000";
				m0ttip = 133415;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 133402;
				gtype = 2;
				area = 133401;
				goal = 133404;
				m0fz = "4029.000000";
				m0widx = 1;
				m1ttip = 133415;
				scitem = -1;
				stype = 2;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 133414;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 133401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "4372211;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 133408;
			gtype = 2;
			oklnk = 2;
			area = 133401;
			goal = 133404;
			sort = 133405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 133402;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

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
				iidx = 835;
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
				iidx0 = 835;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 133407;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;2;";
			type = 0;
		}
	}
}

