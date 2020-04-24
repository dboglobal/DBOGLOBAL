CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 25;
	rq = 0;
	tid = 3787;
	title = 378702;

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
				conv = 378709;
				ctype = 1;
				idx = 5631101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 378708;
				gtype = 1;
				area = 378701;
				goal = 378704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 378705;
				stype = 2;
				taid = 1;
				title = 378702;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 378707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 69;
			}
			CDboTSClickNPC
			{
				npcidx = "5631101;";
			}
			CDboTSCheckClrQst
			{
				and = "3784;";
				flink = 0;
				not = 0;
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
			cid = 253;
			prelnk = "252;0;1;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 378714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 378701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3333201;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 378708;
			gtype = 1;
			oklnk = 2;
			area = 378701;
			goal = 378704;
			sort = 378705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 378702;
		}
	}
}

