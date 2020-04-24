CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2623;
	title = 262302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 262309;
				ctype = 1;
				idx = 5313102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 262308;
				gtype = 1;
				area = 262301;
				goal = 262304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 262305;
				stype = 1;
				taid = 1;
				title = 262302;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 262307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "5313102;";
			}
			CDboTSCheckClrQst
			{
				and = "2622;";
				flink = 1;
				flinknextqid = "2624;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 262308;
			gtype = 1;
			oklnk = 2;
			area = 262301;
			goal = 262304;
			sort = 262305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 262302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 262314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 262301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3612105;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

