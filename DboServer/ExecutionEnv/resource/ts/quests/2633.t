CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 7;
	rq = 0;
	tid = 2633;
	title = 263302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 102;
			elnk = 253;
			nextlnk = 254;
			prelnk = "101;";

			CDboTSActWPSFD
			{
				sid = 2633;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 102;
			prelnk = "100;";

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
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 263308;
			gtype = 1;
			oklnk = 2;
			area = 263301;
			goal = 263304;
			sort = 263305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 263302;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 263309;
				ctype = 1;
				idx = 5313102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 263308;
				gtype = 1;
				area = 263301;
				goal = 263304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 263305;
				stype = 1;
				taid = 1;
				title = 263302;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
				idx0 = 1264;
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
			cid = 253;
			prelnk = "102;1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 263307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckClrQst
			{
				and = "2632;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5313102;";
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 263314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 263301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 1;
				iidx = 1264;
			}
			CDboTSClickNPC
			{
				npcidx = "5313103;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "102;";
			type = 1;
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

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1264;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 263307;
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

