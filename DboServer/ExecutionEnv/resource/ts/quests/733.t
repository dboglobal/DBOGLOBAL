CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 733;
	title = 73302;

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
				conv = 73309;
				ctype = 1;
				idx = 1111103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 73308;
				gtype = 1;
				area = 73301;
				goal = 73304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 73305;
				stype = 1;
				taid = 1;
				title = 73302;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 73314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 73301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511605;";
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
			cont = 73308;
			gtype = 1;
			oklnk = 2;
			area = 73301;
			goal = 73304;
			sort = 73305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 73302;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 73307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 24;
			}
			CDboTSClickNPC
			{
				npcidx = "1111103;";
			}
			CDboTSCheckClrQst
			{
				and = "732;";
				flink = 1;
				not = 0;
			}
		}
	}
}

