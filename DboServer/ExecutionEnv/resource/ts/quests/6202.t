CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6202;
	title = 620202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 620207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 22;
			}
			CDboTSClickNPC
			{
				npcidx = "3031101;";
			}
			CDboTSCheckClrQst
			{
				and = "6201;";
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
			prelnk = "1;0;252;";
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
			cont = 620208;
			gtype = 1;
			oklnk = 2;
			area = 620201;
			goal = 620204;
			sort = 620205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 620202;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 620209;
				ctype = 1;
				idx = 3031101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 620208;
				gtype = 1;
				area = 620201;
				goal = 620204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 620205;
				stype = 2;
				taid = 1;
				title = 620202;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 620214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 620201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3332203;";
			}
		}
	}
}

