CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1717;
	title = 171702;

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
				conv = 171709;
				ctype = 1;
				idx = 5041115;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 171708;
				gtype = 1;
				area = 171701;
				goal = 171704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 171705;
				stype = 2;
				taid = 1;
				title = 171702;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 171707;
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
				npcidx = "5041115;";
			}
			CDboTSCheckClrQst
			{
				and = "1716;";
				flink = 1;
				flinknextqid = "1718;1721;";
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
			cont = 171708;
			gtype = 1;
			oklnk = 2;
			area = 171701;
			goal = 171704;
			sort = 171705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 171702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 171714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 171701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6311102;";
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

