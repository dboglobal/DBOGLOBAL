CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 908;
	title = 90802;

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
				conv = 90809;
				ctype = 1;
				idx = 5061108;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 90808;
				gtype = 1;
				area = 90801;
				goal = 90804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 90805;
				stype = 2;
				taid = 1;
				title = 90802;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 90814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 90801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5061109;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 90808;
			gtype = 1;
			oklnk = 2;
			area = 90801;
			goal = 90804;
			sort = 90805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 90802;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 90807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 31;
			}
			CDboTSClickNPC
			{
				npcidx = "5061108;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "909;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
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

