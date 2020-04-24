CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1210;
	title = 121002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 164;
				taid = 2;
				type = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
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
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 121009;
				ctype = 1;
				idx = 4141105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 121008;
				gtype = 1;
				area = 121001;
				goal = 121004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 121005;
				stype = 1;
				taid = 1;
				title = 121002;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
				idx0 = 164;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
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
			desc = 121014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 121001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 1;
				iidx = 164;
			}
			CDboTSClickNPC
			{
				npcidx = "4111101;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 121008;
			gtype = 1;
			oklnk = 2;
			area = 121001;
			goal = 121004;
			sort = 121005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 121002;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 121007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 41;
			}
			CDboTSClickNPC
			{
				npcidx = "4141105;";
			}
			CDboTSCheckClrQst
			{
				and = "1209;";
				flink = 1;
				flinknextqid = "1468;";
				not = 0;
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
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 121007;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
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
				iidx0 = 164;
				taid = 1;
				type = 1;
			}
		}
	}
}

